#include <vector>
#include <tuple>
#include "utils/CurveInterpolation.hpp"

std::vector<std::tuple<double, double>> interpolate_rate_curve(
  const std::vector<std::tuple<double, double>>& curve_rate,
  const std::vector<double>& curve_cashflow_time) {

  std::vector<std::tuple<double, double>> interpolated_curve;
  size_t j = 0; 

  for (double t : curve_cashflow_time) {
    double interpolated_rate = 0.0;

    if (t <= std::get<0>(curve_rate.front())) {
      interpolated_rate = std::get<1>(curve_rate.front());
    }
    else if (t >= std::get<0>(curve_rate.back())) {
      interpolated_rate = std::get<1>(curve_rate.back());
  }
    else {
      while (j + 1 < curve_rate.size() && std::get<0>(curve_rate[j + 1]) < t) {++j;}
      const auto [t0, r0] = curve_rate[j];
      const auto [t1, r1] = curve_rate[j + 1];

      interpolated_rate = r0 + (r1 - r0) * (t - t0) / (t1 - t0);
    }
  
    interpolated_curve.emplace_back(t, interpolated_rate);
  }

  return interpolated_curve;
}
