#include <gtest/gtest.h>
#include <chrono>
#include "core/InterestRateCurve.hpp"

TEST(InterestRateCurveTest, CheckLoadInterestRateCurveFromCSV){
  
  std::string filepath = std::string(PROJECT_SOURCE_DIR) + "/data/interest_rate_curves/flat_curve.csv";
  std::chrono::sys_days valuation_date = std::chrono::year{2025} / std::chrono::month{1} / std::chrono::day{1}; 
  
  InterestRateCurve curve(filepath, valuation_date);
  
  const std::vector<TupleDateDouble> data = curve.get_curve_data();
  double expected_rate = 0.03;
  
  ASSERT_EQ(curve.get_valuation_date(), valuation_date);
  for(auto [date,rate] : data){
    ASSERT_DOUBLE_EQ(rate, expected_rate);
  }
}


