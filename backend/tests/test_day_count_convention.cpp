#include <gtest/gtest.h>

#include <chrono>
#include "enums/CompoundingConvention.hpp"
#include "enums/DayCountConvention.hpp"
#include "utils/DayCount.hpp"
#include "utils/TimeSchedule.hpp"

TEST(DayCountConventionTest, TimeToMaturity) {
  std::chrono::year_month_day start_ymd{std::chrono::year{2023}, std::chrono::month{4}, std::chrono::day{1}};
  std::chrono::year_month_day end_ymd{std::chrono::year{2023}, std::chrono::month{10}, std::chrono::day{1}};

  std::chrono::sys_days start_date{start_ymd};
  std::chrono::sys_days end_date{end_ymd};

  double actual360 = compute_year_fraction(start_date, end_date, DayCountConvention::Actual360);
  double actual365 = compute_year_fraction(start_date, end_date, DayCountConvention::Actual365);

  EXPECT_DOUBLE_EQ(actual360, 183.0 / 360.0);
  EXPECT_DOUBLE_EQ(actual365, 183.0 / 365.0);
}

TEST(TimeSchedule, DoubleTimeScheduleGenerator_anually){
  double start = 0.0;
  double end = 2.0;
  CompoundingFrequency freq = CompoundingFrequency::anually;
  std::vector<double> schedule = generate_time_schedule(start,end,freq, ShortSchedule::ShortStart);
  std::vector<double> expected_schedule = {0.0, 1.0, 2.0};
  EXPECT_EQ(schedule.size(), expected_schedule.size());
  for(size_t i = 0; i < schedule.size(); ++i){
    EXPECT_EQ(schedule[i], expected_schedule[i]);
  }
}


TEST(TimeSchedule, DoubleTimeScheduleGenerator_semianually){
  double start = 0.0;
  double end = 3.0;
  CompoundingFrequency freq = CompoundingFrequency::semianually;
  std::vector<double> schedule = generate_time_schedule(start,end,freq, ShortSchedule::ShortStart);
  std::vector<double> expected_schedule = {0.0,0.5,1.0,1.5,2.0, 2.5,3.0};
  EXPECT_EQ(schedule.size(), expected_schedule.size());
  for(size_t i = 0; i < schedule.size(); ++i){
    EXPECT_EQ(schedule[i], expected_schedule[i]);
  }
}
