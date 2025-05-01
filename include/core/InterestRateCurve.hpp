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
      const std::vector<TupleDateDouble>& curve_data
    );
    
    InterestRateCurve(
      double valuation_time,
      const std::vector<std::tuple<double,double>>& curve_data
    );
    
  InterestRateCurve(
    const std::string& filepath,
    std::chrono::sys_days valuation_date
  );

  InterestRateCurve(
    const std::string& filepath,
    double valuation_time
  );
  
  virtual ~InterestRateCurve() = default;

    std::variant< std::vector<TupleDateDouble>, std::vector<std::tuple<double,double>> > get_curve_data() const;
    const std::variant<std::chrono::sys_days,double> get_valuation_date() const;

    virtual std::vector<std::tuple<double,double>> interpolate_curve(const std::vector<double>& curve_to_interpolate, DayCountConvention day_count_convention);
  private:
    std::variant<std::chrono::sys_days,double> valuationDate;
    std::variant< std::vector<TupleDateDouble>, std::vector<std::tuple<double,double>> > curve; 

};

#endif
