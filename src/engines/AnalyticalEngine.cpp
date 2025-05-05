#include <chrono>
#include <unordered_map>
#include "engines/AnalyticalEngine.hpp"
#include "core/MarketData.hpp"
#include "enums/CompoundingConvention.hpp"
#include "enums/DayCountConvention.hpp"
#include "models/BlackScholeModel.hpp"
#include "utils/DayCount.hpp"
#include "utils/CurveInterpolation.hpp"
#include "instruments/equity/CallOption.hpp"
#include "enums/ExerciseConvention.hpp"
#include "enums/ModelNames.hpp"
#include "utils/AnalyticalFormulas.hpp"

AnalyticalEngine::AnalyticalEngine(std::shared_ptr<MarketData> market_data)
    : PricingEngine(std::move(market_data)) {}

AnalyticalEngine::AnalyticalEngine(std::shared_ptr<MarketData> market_data, std::shared_ptr<UnderlyingModel> model)
    : PricingEngine(std::move(market_data), std::move(model)) {}

double AnalyticalEngine::compute_price(const Bond& bond) const{
  std::variant<std::vector<TupleDateDouble>, std::vector<std::tuple<double, double>>> curve_data = market_data->get_rate_curve()->get_curve_data();
  DayCountConvention day_convention = bond.get_day_convention(); 
  CompoundingMethod compounding_method = bond.get_compounding_method();
  CompoundingFrequency compounding_frequency = bond.get_compounding_frequency();

  std::variant<std::chrono::sys_days, double> start_date = bond.get_start_date();
  std::variant<std::chrono::sys_days, double> valuation_date = bond.get_valuation_date();
  std::variant<std::chrono::sys_days, double> maturity_date = bond.get_maturity_date();

  // Check if we're using sys_days
  if (std::holds_alternative<std::chrono::sys_days>(start_date) &&
      std::holds_alternative<std::chrono::sys_days>(valuation_date) &&
      std::holds_alternative<std::chrono::sys_days>(maturity_date)) {

      std::chrono::sys_days val = std::get<std::chrono::sys_days>(valuation_date);
      std::chrono::sys_days mat = std::get<std::chrono::sys_days>(maturity_date);

      double valuation_time = compute_year_fraction(val, val, day_convention);
      double maturity_time = compute_year_fraction(val, mat, day_convention);

      auto cashflow_variant = bond.get_vec_futur_cashflows();  // stores the variant locally

      const std::vector<std::chrono::sys_days>& cashflow_dates =
          std::get<std::vector<std::chrono::sys_days>>(cashflow_variant);

      std::vector<double> bond_cashflow_time = compute_year_fraction(val, cashflow_dates, day_convention);

      std::vector<std::tuple<double, double>> curve_time_rate;

      if (std::holds_alternative<std::vector<TupleDateDouble>>(curve_data)) {
          const auto& curve = std::get<std::vector<TupleDateDouble>>(curve_data);
          curve_time_rate = compute_year_fraction(val, curve, day_convention);
      } else {
          curve_time_rate = std::get<std::vector<std::tuple<double, double>>>(curve_data);
      }

      std::vector<std::tuple<double, double>> interpolated_curve = interpolate_rate_curve(curve_time_rate, bond_cashflow_time);

      double nominal = bond.get_nominal();
      double spread = bond.get_spread();
      double coupon_rate = bond.get_coupon_rate();
      double price = 0.0;

      for (size_t i = 0; i < interpolated_curve.size(); ++i) {
          auto [t, r] = interpolated_curve[i];

          if (t >= valuation_time && t <= maturity_time) {
              double yield = spread + r;
              double discount_factor = compute_discount_factor(yield, t, compounding_method, compounding_frequency);

              price += coupon_rate * nominal * discount_factor;

              // Add nominal at maturity
              if (i == interpolated_curve.size() - 1) {
                  price += nominal * discount_factor;
              }
          }
      }

      return price;

  } else {
      throw std::runtime_error("Unsupported date format for bond valuation.");
  }
}

double AnalyticalEngine::compute_price(const CallOption& call) const{

  if (call.get_exercise_kind() == ExerciseKind::American){
std::cout << "not supported" << std::endl;
  }
  switch (this->model->get_model_name()) {
    case ModelName::BlackScholes : {
      auto bs = std::static_pointer_cast<BlackScholesModel>(model);
      double S =  bs->get_spot();
      double sigma = bs->get_sigma();
      double K = call.get_strike();
      double r = bs->get_interest_rate();
      

      std::variant<std::chrono::sys_days, double> valuation_date = call.get_valuation_date();
      std::variant<std::chrono::sys_days, double> maturity_date = call.get_maturity_date();
      double tau;
      
      if (std::holds_alternative<std::chrono::sys_days>(valuation_date) &&
          std::holds_alternative<std::chrono::sys_days>(maturity_date)) {

        std::chrono::sys_days val = std::get<std::chrono::sys_days>(valuation_date);
        std::chrono::sys_days mat = std::get<std::chrono::sys_days>(maturity_date);
        tau = compute_year_fraction(val, mat, call.get_day_convention());
      }

      else if (std::holds_alternative<double>(valuation_date) &&
                 std::holds_alternative<double>(maturity_date)) {
        double val = std::get<double>(valuation_date);
        double mat = std::get<double>(maturity_date);
        tau = compute_year_fraction(val, mat); 
      }

      else {
        throw std::runtime_error("Mismatched date types for valuation and maturity.");
      }

      double price = compute_bs_call_price(S,K,tau,sigma,r);
      return price;
    }
    case ModelName::Heston : {
      return 0.0;
    }
  }
}

