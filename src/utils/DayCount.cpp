#include <chrono>
#include <vector>
#include <tuple>
#include "utils/DayCount.hpp"
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
  }
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
  
  std::vector<double> vec_time(vec_date.size(), {0.0});
  for (const auto& date : vec_date) {
          double yf = compute_year_fraction(valuation_date, date, convention);
          vec_time.emplace_back(yf);
      } 
  return vec_time;
}

double compute_discount_factor(
  double yield,
  double time_to_maturity,
  CompoundingConvention compounding_convention){
  
  double df = 0.0;
  switch (compounding_convention) {
    case CompoundingConvention::continuous:
      df = std::exp(-yield*time_to_maturity);
    case CompoundingConvention::actuarial_anually:
      df = std::pow(1+yield,-time_to_maturity);
    case CompoundingConvention::actuarial_semianually:
      df = std::pow(1+yield/2.0,-2.0*time_to_maturity);
    case CompoundingConvention::actuarial_quarterly:
      df = std::pow(1+yield/4.0,-4.0*time_to_maturity);
    case CompoundingConvention::actuarial_monthly:
      df = std::pow(1+yield/12.0,-12.0*time_to_maturity);
  }
  return df;
}
