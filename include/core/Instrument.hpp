#ifndef INSTRUMENT_HPP
#define INSTRUMENT_HPP

#include <string>

class PricingEngine;

class Instrument {
  public:
    virtual ~Instrument() = default;
    
    virtual void setPricingEngine(std::shared_ptr<PricingEngine> engine) = 0;
    std::string get_name();
    virtual double compute_price() const = 0;
  private:
    std::string name;
};


#endif
