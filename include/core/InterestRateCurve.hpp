#ifndef INTERESTRATECURVES_HPP
#define INTERESTRATECURVES_HPP

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

    virtual double interpolate(std::chrono::sys_days date) const = 0;
    virtual std::vector<TupleDateDouble> interpolate(
      const std::vector<TupleDateDouble>& vec_rates) const = 0;

    const std::vector<TupleDateDouble>& get_curve() const;
    std::chrono::sys_days get_valuation_date() const;
  private:
    std::chrono::sys_days valuationDate;
    std::vector<TupleDateDouble> curve;
     
};

#endif
