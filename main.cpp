#include <iostream>
#include <chrono>
#include "enums/CompoundingConvention.hpp"
#include "enums/DayCountConvention.hpp"
#include "instruments/Bond.hpp"
#include "utils/DayCount.hpp"
#include "core/InterestRateCurve.hpp"
#include "utils/CurveInterpolation.hpp"

int main(){
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

  
  std::chrono::sys_days start = std::chrono::year{2025}/5/1;  // First coupon
  std::chrono::sys_days end = std::chrono::year{2026}/5/1;  // Third coupon
  std::cout << start << std::endl;
  std::cout << end << std::endl;
  std::cout << compute_year_fraction(start,end,DayCountConvention::Actual360) << std::endl;
}
