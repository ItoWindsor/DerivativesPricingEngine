#ifndef INTERESTRATECURVES_HPP
#define INTERESTRATECURVES_HPP

#include "enums/DayCountConvention.hpp"
#include <chrono>
#include <vector> 
#include <tuple>

using TupleDateDouble = std::tuple<std::chrono::sys_days, double>;

class InterestRateCurve {
  public:
    InterestRateCurve(
      std::chrono::sys_days valuation_date,
      const std::vector<TupleDateDouble>& curve_data);
    
    InterestRateCurve(
      const std::string& filepath, std::chrono::sys_days valuation_date
    );

    virtual ~InterestRateCurve() = default;

    const std::vector<TupleDateDouble>& get_curve_data() const;
    const std::chrono::sys_days get_valuation_date() const;

    virtual std::vector<std::tuple<double,double>> interpolate_curve(const std::vector<double>& curve_to_interpolate, DayCountConvention day_count_convention);
  private:
    std::chrono::sys_days valuationDate;
    std::vector<TupleDateDouble> curve;
     
};

#endif
