#ifndef CURVEINTERPOLATION_HPP
#define CURVEINTERPOLATION_HPP

#include <vector>
#include <tuple>

std::vector<std::tuple<double,double>> interpolate_rate_curve(
  const std::vector<std::tuple<double,double>>& curve_rate,
  const std::vector<double>& curve_cashflow_time);
  
#endif
