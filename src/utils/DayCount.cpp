#include <chrono>
#include <vector>
#include <tuple>
#include "utils/DayCount.hpp"
#include "core/InterestRateCurve.hpp"
#include "enums/DayCountConvention.hpp"

double year_fraction(
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

std::vector<std::tuple<double,double>>  year_fraction(
  std::chrono::sys_days valuation_date,
  const InterestRateCurve& curve,
  DayCountConvention convention
  ) {
  
  auto original_curve = curve.get_curve();
  std::vector<std::tuple<double,double>> vec_data_curve(original_curve.size(), {0.0,0.0});
  for (const auto& [date, rate] : original_curve) {
          double yf = year_fraction(valuation_date, date, convention);
          vec_data_curve.emplace_back(yf, rate);
      }
  
  return vec_data_curve;
}
