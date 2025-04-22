#include <gtest/gtest.h>
#include "instruments/Bond.hpp"
#include "core/MarketData.hpp"
#include "engines/AnalyticalEngine.hpp"
#include "enums/CompoundingConvention.hpp"
#include "enums/DayCountConvention.hpp"
#include "utils/TimeSchedule.hpp"

TEST(BondPricingTest, PriceBond_OneCashflow_FlatCurve_DirtyPrice) {
  std::string filepath = std::string(PROJECT_SOURCE_DIR) + "/data/interest_rate_curves/flat_curve.csv";
  std::chrono::sys_days start_date = std::chrono::year{2024}/5/1;
  std::chrono::sys_days valuation_date = start_date;
  std::chrono::sys_days maturity_date = std::chrono::year{2025}/1/10;
  
  double face_value = 100;
  double coupon_rate = 0.04;
  double spread = 0.0;
  
  auto market_data = std::make_shared<MarketData>();
  market_data->load_interest_rate_curve(filepath, valuation_date);

  AnalyticalEngine engine(market_data);

  std::vector<std::chrono::sys_days> cashflow_dates = {
      maturity_date
  };

  Bond bond(
    start_date,
    valuation_date,
    maturity_date,
    face_value,
    coupon_rate,
    spread,
    cashflow_dates,
    DayCountConvention::Actual360,
    CompoundingMethod::actuarial,
    CompoundingFrequency::anually);

  double price = engine.compute_price(bond);
  double expected_price = 102.4359378;
  ASSERT_NEAR(price, expected_price, 1e-7);
}

TEST(BondPricingTest, PriceBond_MultipleCashflows_FlatCurve_DirtyPrice) {
  std::string filepath = std::string(PROJECT_SOURCE_DIR) + "/data/interest_rate_curves/flat_curve.csv";
  std::chrono::sys_days start_date = std::chrono::year{2024}/5/1;
  std::chrono::sys_days valuation_date = start_date;
  std::chrono::sys_days maturity_date = std::chrono::year{2027}/5/1;
  
  double face_value = 100;
  double coupon_rate = 0.04;
  double spread = 0.0;
  
  auto market_data = std::make_shared<MarketData>();
  market_data->load_interest_rate_curve(filepath, valuation_date);

  AnalyticalEngine engine(market_data);

  // Multiple cashflow dates
  std::vector<std::chrono::sys_days> cashflow_dates = {
      std::chrono::year{2025}/5/1,  // First coupon
      std::chrono::year{2026}/5/1,  // Third coupon
      maturity_date                 // Principal repayment
  };

  Bond bond(
    start_date,
    valuation_date,
    maturity_date,
    face_value,
    coupon_rate,
    spread,
    cashflow_dates,
    DayCountConvention::Actual360,
    CompoundingMethod::actuarial,
    CompoundingFrequency::anually);

  double price = engine.compute_price(bond);
  double expected_price = 102.7067762; // Based on the 0.03 flat curve
  ASSERT_NEAR(price, expected_price, 1e-3);
}
