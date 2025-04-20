#ifndef DAYCOUNT_HPP
#define DAYCOUNT_HPP

#include <chrono>
#include <tuple>
#include <vector>
#include "enums/DayCountConvention.hpp"
#include "enums/CompoundingConvention.hpp"

double compute_year_fraction(
  std::chrono::sys_days start,
  std::chrono::sys_days end,
  DayCountConvention convention);


std::vector<std::tuple<double,double>> compute_year_fraction(
  std::chrono::sys_days valuation_date,
  const std::vector<std::tuple<std::chrono::sys_days,double>>& curve_data,
  DayCountConvention convention);

std::vector<double> compute_year_fraction(
  std::chrono::sys_days valuation_date,
  const std::vector<std::chrono::sys_days>& vec_date, 
  DayCountConvention convention);


#endif

double compute_discount_factor( 
  double yield,
  double time_to_maturity,
  CompoundingConvention compounding_convention
);
