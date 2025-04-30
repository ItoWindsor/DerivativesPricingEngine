#include "curves/FlatCurve.hpp"
#include "core/InterestRateCurve.hpp"
#include "enums/DayCountConvention.hpp"

std::vector<TupleDateDouble> FlatCurve::create_curve_data(std::chrono::sys_days maturity_date, double r) {
        std::vector<TupleDateDouble> curve_data;
        curve_data.emplace_back(maturity_date, r);
        return curve_data;
    }

FlatCurve::FlatCurve(
  std::chrono::sys_days valuation_date,
  std::chrono::sys_days maturity_date,
  double r)
  : InterestRateCurve(valuation_date, create_curve_data(maturity_date,r)) {}


std::vector<std::tuple<double,double>> FlatCurve::interpolate_curve(const std::vector<double>& curve_to_interpolate, DayCountConvention day_convention){

  double r = std::get<1>(this->get_curve_data().front());
  
  std::vector<std::tuple<double, double>> interpolated_curve;
  for (double t : curve_to_interpolate){
    interpolated_curve.emplace_back(t,r);
  }

  return interpolated_curve;
}
