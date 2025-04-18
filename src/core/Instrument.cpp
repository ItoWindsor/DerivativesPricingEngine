#include <chrono>
#include "core/Instrument.hpp"

Instrument::Instrument(
    std::chrono::sys_days start_date,
    std::chrono::sys_days valuation_date,
    std::chrono::sys_days maturity_date,
    std::string name
  )
  : startDate(start_date), valuationDate(valuation_date), maturityDate(maturity_date), name(name) {}

std::string Instrument::get_name() {
  return this->name;
}

std::chrono::sys_days Instrument::get_start_date() const{
  return this->startDate;
}


std::chrono::sys_days Instrument::get_valuation_date() const{
  return this->valuationDate;
}

std::chrono::sys_days Instrument::get_maturity_date() const{
  return this->maturityDate;
}
