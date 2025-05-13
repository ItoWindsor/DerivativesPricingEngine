#include <chrono>
#include <vector>
#include <tuple>
#include "utils/DayCount.hpp"
#include "enums/CompoundingConvention.hpp"
#include "enums/DayCountConvention.hpp"

double compute_year_fraction(
  std::chrono::sys_days start,
  std::chrono::sys_days end,
  DayCountConvention convention) {

  std::chrono::days days_duration = std::chrono::duration_cast<std::chrono::days>(end - start);
  double days = days_duration.count();

  switch (convention) {
      case DayCountConvention::Actual360:
          return static_cast<double>(days) / 360.0;
      case DayCountConvention::Actual365:
          return static_cast<double>(days) / 365.0;
      case DayCountConvention::Actual:
          return static_cast<double>(days);
  }
}


double compute_year_fraction(
  double start,
  double end) {

  return end - start;
}

std::vector<std::tuple<double,double>> compute_year_fraction(
  std::chrono::sys_days valuation_date,
  const std::vector<std::tuple<std::chrono::sys_days,double>>& curve_data,
  DayCountConvention convention
  ) {
  
  std::vector<std::tuple<double,double>> vec_data_curve(curve_data.size(), {0.0,0.0});
  for (const auto& [date, rate] : curve_data) {
          double yf = compute_year_fraction(valuation_date, date, convention);
          vec_data_curve.emplace_back(yf, rate);
      } 
  return vec_data_curve;
}


std::vector<double> compute_year_fraction(
  std::chrono::sys_days valuation_date,
  const std::vector<std::chrono::sys_days>& vec_date,
  DayCountConvention convention
  ) {
  
  std::vector<double> vec_time;
  for (const auto& date : vec_date) {
          double yf = compute_year_fraction(valuation_date, date, convention);
          vec_time.emplace_back(yf);
      } 
  return vec_time;
}

double compute_discount_factor(
  double yield,
  double time_to_maturity,
  CompoundingMethod compounding_method,
  CompoundingFrequency compounding_frequency) {

  double df = 0.0;

  switch (compounding_method) {
    case CompoundingMethod::continuous:
      df = std::exp(-yield * time_to_maturity);
      break;

    case CompoundingMethod::actuarial:
      switch (compounding_frequency) {
        case CompoundingFrequency::anually:
          df = std::pow(1 + yield, -time_to_maturity);
          break;
        case CompoundingFrequency::semianually:
          df = std::pow(1 + (yield/2.0), -2.0 * time_to_maturity);
          break;
        case CompoundingFrequency::quarterly:
          df = std::pow(1 + (yield / 4.0), -4.0 * time_to_maturity);
          break;
        case CompoundingFrequency::monthly:
          df = std::pow(1 + (yield/12.0), -12.0 * time_to_maturity);
          break;
      }
      break;
  }

  return df;
}

std::tuple<double, double> from_date_to_double(
  std::chrono::sys_days valuation_date,
  std::chrono::sys_days maturity_date,
  DayCountConvention day_convention){
  
  double valuation_time = compute_year_fraction(valuation_date, valuation_date, day_convention);
  double maturity_time = compute_year_fraction(valuation_date, maturity_date, day_convention);

  return std::make_tuple(valuation_time, maturity_time);
}
