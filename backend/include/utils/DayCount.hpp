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


double compute_year_fraction(
 double start,
 double end);

std::vector<std::tuple<double,double>> compute_year_fraction(
  std::chrono::sys_days valuation_date,
  const std::vector<std::tuple<std::chrono::sys_days,double>>& curve_data,
  DayCountConvention convention);

std::vector<double> compute_year_fraction(
  std::chrono::sys_days valuation_date,
  const std::vector<std::chrono::sys_days>& vec_date, 
  DayCountConvention convention);

double compute_discount_factor( 
  double yield,
  double time_to_maturity,
  CompoundingMethod compounding_convention,
  CompoundingFrequency compouding_frequency
);

#endif

std::tuple<double, double> from_date_to_double(
  std::chrono::sys_days valuation_date,
  std::chrono::sys_days maturity_date,
  DayCountConvention day_convention);
