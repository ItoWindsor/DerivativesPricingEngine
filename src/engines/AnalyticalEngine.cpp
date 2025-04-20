#include <chrono>
#include "engines/AnalyticalEngine.hpp"
#include "core/MarketData.hpp"
#include "enums/CompoundingConvention.hpp"
#include "enums/DayCountConvention.hpp"
#include "utils/DayCount.hpp"
#include "utils/CurveInterpolation.hpp"

AnalyticalEngine::AnalyticalEngine(std::shared_ptr<MarketData> market_data)
    : market_data(std::move(market_data)) {}

double AnalyticalEngine::compute_price(const Bond& bond){

  std::vector<TupleDateDouble> curve_data = market_data->get_rate_curve()->get_curve_data();
  DayCountConvention day_convention = bond.get_day_convention(); 
  CompoundingConvention compounding_convention = bond.get_compounding_convention();
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
        double discount_factor = compute_discount_factor(yield, t, compounding_convention);
        price += coupon_rate*nominal*discount_factor; 
        if (t == std::get<0>(interpolated_curve.back()) ){
          price += nominal*discount_factor;
        }
      }
  }

  return price;
}
