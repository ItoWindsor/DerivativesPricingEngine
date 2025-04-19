#include <gtest/gtest.h>

#include <chrono>
#include "enums/DayCountConvention.hpp"
#include "utils/DayCount.hpp"

TEST(DayCountConventionTest, TimeToMaturity) {
  std::chrono::year_month_day start_ymd{std::chrono::year{2023}, std::chrono::month{4}, std::chrono::day{1}};
  std::chrono::year_month_day end_ymd{std::chrono::year{2023}, std::chrono::month{10}, std::chrono::day{1}};

  std::chrono::sys_days start_date{start_ymd};
  std::chrono::sys_days end_date{end_ymd};

  double actual360 = year_fraction(start_date, end_date, DayCountConvention::Actual360);
  double actual365 = year_fraction(start_date, end_date, DayCountConvention::Actual365);

  EXPECT_DOUBLE_EQ(actual360, 183.0 / 360.0);
  EXPECT_DOUBLE_EQ(actual365, 183.0 / 365.0);
}

