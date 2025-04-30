#ifndef FLATCURVE_HPP
#define FLATCURVE_HPP

#include <chrono>
#include "core/InterestRateCurve.hpp"
#include "enums/DayCountConvention.hpp"

class FlatCurve : public InterestRateCurve {
  public:
    FlatCurve(
      std::chrono::sys_days valuation_date,
      std::chrono::sys_days maturity_date,
      double r);
    
    std::vector<std::tuple<double,double>> interpolate_curve(const std::vector<double>& curve_to_interpolate, DayCountConvention day_convention = DayCountConvention::Actual360) override;
  private:
    static std::vector<TupleDateDouble> create_curve_data(std::chrono::sys_days maturity_date, double r);
};

#endif
