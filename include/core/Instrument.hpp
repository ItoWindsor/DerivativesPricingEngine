#ifndef INSTRUMENT_HPP
#define INSTRUMENT_HPP

#include <string>

class PricingEngine;

class Instrument {
public :
  virtual ~Instrument() = default;
  virtual std::string name() const = 0;
  virtual void setPricingEngine(std::shared_ptr<PricingEngine> engine) = 0;
  
  virtual double compute_price() const = 0;
};


#endif
