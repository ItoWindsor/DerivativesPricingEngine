#include <gtest/gtest.h>
#include <chrono>
#include "core/InterestRateCurve.hpp"
#include "utils/CurveInterpolation.hpp"

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

TEST(InterestRateCurveTest, InterpolationFlatCurve){

  std::string filepath = std::string(PROJECT_SOURCE_DIR) + "/data/interest_rate_curves/flat_curve.csv";
  std::chrono::sys_days valuation_date = std::chrono::year{2025} / std::chrono::month{1} / std::chrono::day{1}; 
  
  InterestRateCurve curve(filepath, valuation_date);
  
}

TEST(InterpolationTest, LinearInterpolation) {
  std::vector<std::tuple<double, double>> curve = {
      {0.0, 0.02},
      {1.0, 0.04},
      {2.0, 0.06}
  };
  std::vector<double> query = {0.5, 1.5, 2.0};

  std::vector<std::tuple<double,double>> interpolated = interpolate_rate_curve(curve, query);
  ASSERT_EQ(interpolated.size(), query.size());

  std::vector<double> expected = {0.03, 0.05, 0.06};
  for (size_t i = 0; i < query.size(); ++i) {
    ASSERT_NEAR(std::get<1>(interpolated[i]), expected[i], 1e-6);
  }
}

TEST(InterpolationTest, FlatExtrapolationBeforeFirst) {
  std::vector<std::tuple<double, double>> curve = {
      {1.0, 0.03},
      {2.0, 0.05}
  };
  std::vector<double> query = {0.5};

  std::vector<std::tuple<double,double>>  interpolated = interpolate_rate_curve(curve, query);
  ASSERT_NEAR(std::get<1>(interpolated[0]), 0.03, 1e-6);
}

TEST(InterpolationTest, FlatExtrapolationAfterLast) {
  std::vector<std::tuple<double, double>> curve = {
      {1.0, 0.03},
      {2.0, 0.05}
  };
  std::vector<double> query = {3.0};

  std::vector<std::tuple<double,double>>  interpolated = interpolate_rate_curve(curve, query);
  ASSERT_NEAR(std::get<1>(interpolated[0]), 0.05, 1e-6);
}


