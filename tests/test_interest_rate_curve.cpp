#include <gtest/gtest.h>
#include <chrono>
#include "core/InterestRateCurve.hpp"
#include "utils/CurveInterpolation.hpp"
#include "enums/DayCountConvention.hpp"
#include "utils/DayCount.hpp"


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
  ASSERT_NEAR(std::get<1>(interpolated[0]), 0.05, 1e-10);
}

TEST(InterestRateCurveTest, InterpolationOnCurveExample) {
  std::string filepath = std::string(PROJECT_SOURCE_DIR) + "/data/interest_rate_curves/curve_example.csv";
  std::chrono::sys_days valuation_date = std::chrono::year{2024}/5/10;

  InterestRateCurve curve(filepath, valuation_date);
  auto curve_data = curve.get_curve_data();

  auto curve_time_rate = compute_year_fraction(valuation_date, curve_data, DayCountConvention::Actual360);

  std::vector<std::chrono::sys_days> test_dates = {
    std::chrono::year{2024}/7/10,  // Between 2024-05-10 and 2024-10-10
    std::chrono::year{2024}/12/10, // Between 2024-10-10 and 2025-01-10
    std::chrono::year{2029}/1/1    // Between 2025-01-10 and 2030-04-17
  };

  auto test_times = compute_year_fraction(valuation_date, test_dates, DayCountConvention::Actual360);
  auto interpolated = interpolate_rate_curve(curve_time_rate, test_times);

  ASSERT_EQ(interpolated.size(), test_times.size());

  // Expected values from linear interpolation:
  // 1. Between 0.05 and 0.03 → expect ≈ 0.04
  // 2. Between 0.03 and 0.02 → expect ≈ 0.025
  // 3. Between 0.02 and 0.03 → expect ≈ 0.0255 (depends on year fractions)

  std::vector<double> expected_rates = {0.0420294, 0.0233696, 0.0275507};

  for (size_t i = 0; i < expected_rates.size(); ++i) {
    double interpolated_value = std::get<1>(interpolated[i]);
    double expected_value = expected_rates[i];
    ASSERT_NEAR(interpolated_value, expected_value, 1e-5); // Allow small tolerance
  }
}
