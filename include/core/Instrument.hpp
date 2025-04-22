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
    std::string get_name();

    std::chrono::sys_days get_start_date() const;
    std::chrono::sys_days get_valuation_date() const;
    std::chrono::sys_days get_maturity_date() const;
    DayCountConvention get_day_convention() const;
  private:
    std::chrono::sys_days startDate;
    std::chrono::sys_days valuationDate;
    std::chrono::sys_days maturityDate;
    DayCountConvention dayCountConvention;
};


#endif
