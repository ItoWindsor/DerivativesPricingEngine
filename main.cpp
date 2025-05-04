#include <iostream>
#include <chrono>
#include "core/MarketData.hpp"
#include "enums/CompoundingConvention.hpp"
#include "enums/DayCountConvention.hpp"
#include "instruments/Bond.hpp"
#include "instruments/equity/CallOption.hpp"
#include "engines/AnalyticalEngine.hpp"
#include "models/BlackScholeModel.hpp"
#include "utils/DayCount.hpp"
#include "core/InterestRateCurve.hpp"
#include "utils/CurveInterpolation.hpp"

int main(){
  AnalyticalEngine engine;
  std::shared_ptr<MarketData> market_data = std::make_shared<MarketData>();
  std::shared_ptr<BlackScholesModel> bs_model = std::make_shared<BlackScholesModel>();

  double S0 = 100;
  double K = 100;
  double sigma = 0.2;
  double r = 0.02;
  double t = 0;
  double T = 1;

  ExerciseKind exercise_kind = ExerciseKind::European;

  bs_model->set_r(r);
  bs_model->set_sigma(sigma);
  bs_model->set_spot(S0);

  engine.set_model(bs_model);
  engine.set_market_data(market_data);

  CallOption call = CallOption(t,t,T,K,exercise_kind, false);

  double price = engine.compute_price(call);
  std::cout << "the price of the call is : " << price << std::endl;

  //std::chrono::sys_days start_date = std::chrono::year{2025} / std::chrono::month{1} / std::chrono::day{1}; 
  //std::chrono::sys_days valuation_date = start_date;
  //std::chrono::sys_days maturity_date = std::chrono::year{2025} / std::chrono::month{5} / std::chrono::day{12};
  //CompoundingFrequency compounding_frequency = CompoundingFrequency::monthly;
  //double nominal = 100;
  //double coupon_rate = 0.02;
  //double spread = 0.0;
  //
  //Bond bond(
  //start_date,
  //valuation_date,
  //maturity_date,
  //nominal,
  //coupon_rate,
  //spread,
  //DayCountConvention::Actual365,           
  //CompoundingMethod::actuarial,            
  //compounding_frequency,           
  //ShortSchedule::ShortEnd                 
  //);

  //std::cout << bond << std::endl;

  
}
