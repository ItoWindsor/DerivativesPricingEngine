#include <chrono>
#include "core/Instrument.hpp"
#include "enums/DayCountConvention.hpp"

Instrument::Instrument(
    std::chrono::sys_days start_date,
    std::chrono::sys_days valuation_date,
    std::chrono::sys_days maturity_date,
    DayCountConvention dayCountConvention
  )
  : startDate(start_date), valuationDate(valuation_date), maturityDate(maturity_date), dayCountConvention(dayCountConvention) {}

Instrument::Instrument(
    double start_time,
    double valuation_time,
    double maturity_time)
  : startDate(start_time), valuationDate(valuation_time), maturityDate(maturity_time), dayCountConvention(DayCountConvention::Actual) {}

std::variant<std::chrono::sys_days,double> Instrument::get_start_date() const{
  return this->startDate;
}

std::variant<std::chrono::sys_days,double> Instrument::get_valuation_date() const{
  return this->valuationDate;
}

std::variant<std::chrono::sys_days,double> Instrument::get_maturity_date() const{
  return this->maturityDate;
}

DayCountConvention Instrument::get_day_convention() const{
  return this->dayCountConvention;
}
