#include "curves/FlatCurve.hpp"
#include "core/InterestRateCurve.hpp"
#include "enums/DayCountConvention.hpp"

std::vector<TupleDateDouble> FlatCurve::create_curve_data(std::chrono::sys_days maturity_date, double r) {
  std::vector<TupleDateDouble> curve_data;
  curve_data.emplace_back(maturity_date, r);
  return curve_data;
}

std::vector<std::tuple<double,double>> FlatCurve::create_curve_data(double maturity_date, double r) {
  std::vector<std::tuple<double,double>> curve_data;
  curve_data.emplace_back(maturity_date, r);
  return curve_data;
}

FlatCurve::FlatCurve(
  std::chrono::sys_days valuation_date,
  std::chrono::sys_days maturity_date,
  double r)
  : InterestRateCurve(valuation_date, create_curve_data(maturity_date,r)) {}


FlatCurve::FlatCurve(
  double valuation_time,
  double maturity_time,
  double r)
  : InterestRateCurve(valuation_time, create_curve_data(maturity_time,r)) {}

std::vector<std::tuple<double, double>> FlatCurve::interpolate_curve(
    const std::vector<double>& curve_to_interpolate,
    DayCountConvention /* day_convention */) 
{
  const auto& data_variant = this->get_curve_data();

  double r;

  if (std::holds_alternative<std::vector<std::tuple<double,double>>>(data_variant)) {
      const auto& data = std::get<std::vector<std::tuple<double,double>>>(data_variant);
      if (data.empty()) {
          throw std::runtime_error("FlatCurve time-based data is empty.");
      }
      r = std::get<1>(data.front());
  } else if (std::holds_alternative<std::vector<TupleDateDouble>>(data_variant)) {
      const auto& data = std::get<std::vector<TupleDateDouble>>(data_variant);
      if (data.empty()) {
          throw std::runtime_error("FlatCurve date-based data is empty.");
      }
      r = std::get<1>(data.front());
  } else {
      throw std::runtime_error("Unsupported curve type in FlatCurve.");
  }

  std::vector<std::tuple<double, double>> interpolated_curve;
  interpolated_curve.reserve(curve_to_interpolate.size());

  for (double t : curve_to_interpolate) {
      interpolated_curve.emplace_back(t, r);
  }

  return interpolated_curve;
}
