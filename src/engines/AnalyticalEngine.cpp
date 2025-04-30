#include <chrono>
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

AnalyticalEngine::AnalyticalEngine(std::shared_ptr<MarketData> market_data)
    : market_data(std::move(market_data)) {}

void AnalyticalEngine::set_model(std::shared_ptr<UnderlyingModel> model) {
    this->model = std::move(model);
}

void AnalyticalEngine::set_market_data(std::shared_ptr<MarketData> market_data) {
    this->market_data = std::move(market_data);
}

double AnalyticalEngine::compute_price(const Bond& bond){

  std::vector<TupleDateDouble> curve_data = market_data->get_rate_curve()->get_curve_data();
  DayCountConvention day_convention = bond.get_day_convention(); 
  CompoundingMethod compounding_method = bond.get_compounding_method();
  CompoundingFrequency compounding_frequency = bond.get_compounding_frequency();
 
  std::chrono::sys_days valuation_date = bond.get_valuation_date();
  std::chrono::sys_days maturity_date = bond.get_maturity_date();
 
  double valuation_time = compute_year_fraction(valuation_date, valuation_date, day_convention);
  double maturity_time = compute_year_fraction(valuation_date, maturity_date, day_convention);

  std::vector<double> bond_cashflow_time = compute_year_fraction(valuation_date,bond.get_vec_date_cashflows(), day_convention); 
  std::vector<std::tuple<double,double>> curve_time_rate = compute_year_fraction(valuation_date, curve_data, day_convention);
  std::vector<std::tuple<double,double>> interpolated_curve = interpolate_rate_curve(curve_time_rate,bond_cashflow_time);

  double nominal = bond.get_nominal();
  double spread = bond.get_spread();
  double coupon_rate = bond.get_coupon_rate();
  double price = 0.0;

  for(auto [t, r] : interpolated_curve){
  
    if (t >= valuation_time || t <= maturity_time){
    
      double yield = spread + r;
      double discount_factor = compute_discount_factor(yield, t, compounding_method, compounding_frequency);
      
      price += coupon_rate*nominal*discount_factor; 
        

      if (t == std::get<0>(interpolated_curve.back()) ){
          price += nominal*discount_factor;
        }
      
    }
  }

  return price;
}

double AnalyticalEngine::compute_price(const CallOption& call){

  this->model->fit(*(this->market_data), "AAPL");
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
      
      double T = compute_year_fraction(
        call.get_valuation_date(),
        call.get_maturity_date(),
        call.get_day_convention());

      double d1 = (std::log(S / K) + (r + 0.5 * sigma * sigma) * T) / (sigma * std::sqrt(T));
      double d2 = d1 - sigma * std::sqrt(T);

      double Nd1 = 0.5 * (1 + std::erf(d1 / std::sqrt(2)));
      double Nd2 = 0.5 * (1 + std::erf(d2 / std::sqrt(2)));

      return S * Nd1 - K * std::exp(-r * T) * Nd2;
    }
    case ModelName::Heston : {
      return 0.0;
    }
  }
}





