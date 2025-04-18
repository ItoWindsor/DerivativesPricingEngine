#ifndef INSTRUMENT_HPP
#define INSTRUMENT_HPP

#include <string>
#include <chrono>
#include <iostream>

class PricingEngine;

class Instrument {
  public:
    virtual ~Instrument() = default;
    
    virtual void setPricingEngine(std::shared_ptr<PricingEngine> engine) = 0;
    std::string get_name();
    virtual double compute_price() const = 0;
  private:
    std::chrono::year_month_day StartDate;
    std::chrono::year_month_day ValuationDate;
    std::chrono::year_month_day MaturityDate;
    std::string name;
};


#endif
