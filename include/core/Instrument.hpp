#ifndef INSTRUMENT_HPP
#define INSTRUMENT_HPP

#include <string>
#include <chrono>

class PricingEngine;

class Instrument {
  public:
    virtual ~Instrument() = default;
    
    virtual void setPricingEngine(std::shared_ptr<PricingEngine> engine) = 0;
    std::string get_name();

    std::chrono::sys_days get_start_date() const;
    std::chrono::sys_days get_valuation_date() const;
    std::chrono::sys_days get_maturity_date() const;
  private:
    std::chrono::sys_days StartDate;
    std::chrono::sys_days ValuationDate;
    std::chrono::sys_days MaturityDate;
    std::string name;
};


#endif
