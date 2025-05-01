#ifndef INSTRUMENT_HPP
#define INSTRUMENT_HPP

#include "enums/DayCountConvention.hpp"
#include <string>
#include <chrono>

class PricingEngine;

class Instrument {
  public:
    Instrument(
      std::chrono::sys_days start_date,
      std::chrono::sys_days valuation_date,
      std::chrono::sys_days maturity_date,
      DayCountConvention dayCountConvention = DayCountConvention::Actual365
      );

    Instrument(
      double start_time,
      double valuation_time,
      double maturity_time);

    std::string get_name();

    std::variant<std::chrono::sys_days,double> get_start_date() const;
    std::variant<std::chrono::sys_days,double> get_valuation_date() const;
    std::variant<std::chrono::sys_days,double> get_maturity_date() const;
    DayCountConvention get_day_convention() const;
  private:
    std::variant<std::chrono::sys_days,double> startDate;
    std::variant<std::chrono::sys_days,double> valuationDate;
    std::variant<std::chrono::sys_days,double> maturityDate;
    DayCountConvention dayCountConvention;
};


#endif
