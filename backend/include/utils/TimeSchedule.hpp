#include <vector>
#include <chrono>
#include "enums/CompoundingConvention.hpp"
#include "enums/DayCountConvention.hpp"

std::vector<std::chrono::sys_days> generate_time_schedule(
  std::chrono::sys_days start_date,
  std::chrono::sys_days end_date,
  CompoundingFrequency compounding_frequency,
  ShortSchedule short_schedule
);


std::vector<double> generate_time_schedule(
  double start_date,
  double end_date,
  CompoundingFrequency compounding_frequency,
  ShortSchedule short_schedule
);


std::chrono::year_month_day add_period(
  const std::chrono::year_month_day& date,
  CompoundingFrequency freq);


std::chrono::year_month_day subtract_period(
  const std::chrono::year_month_day& date,
  CompoundingFrequency freq);

std::chrono::sys_days make_date(int year, unsigned int month, unsigned int day);
