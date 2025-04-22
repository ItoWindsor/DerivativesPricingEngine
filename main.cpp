#include <iostream>
#include <chrono>
#include "enums/CompoundingConvention.hpp"
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


  std::string filepath = std::string(PROJECT_SOURCE_DIR) + "/data/interest_rate_curves/curve_example.csv";
  std::chrono::sys_days valuation_date = std::chrono::year{2024}/5/10;

  InterestRateCurve curve(filepath, valuation_date);
  auto curve_data = curve.get_curve_data();

  for (const auto& [date, rate] : curve.get_curve_data()) {
      std::cout << "Date: " << date << ", Rate: " << rate << std::endl;
  }

  auto curve_time_rate = compute_year_fraction(valuation_date, curve_data, DayCountConvention::Actual360);

  std::vector<std::chrono::sys_days> test_dates = {
    std::chrono::year{2024}/7/10,  // Between 2024-05-10 and 2024-10-10
    std::chrono::year{2024}/12/10, // Between 2024-10-10 and 2025-01-10
    std::chrono::year{2029}/1/1,    // Between 2025-01-10 and 2030-04-17
    std::chrono::year{2036}/1/1
  };

  auto test_times = compute_year_fraction(valuation_date, test_dates, DayCountConvention::Actual360);
  auto interpolated = interpolate_rate_curve(curve_time_rate, test_times);
  for(auto& [t,r] : interpolated){
    std::cout << "  Time: " << t << ", Rate: " << r << "\n";
  }
}
