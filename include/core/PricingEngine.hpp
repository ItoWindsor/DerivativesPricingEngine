#ifndef PRICINGENGINE_HPP
#define PRICINGENGINE_HPP

#include "core/MarketData.hpp"
#include <memory>

class Instrument;
class Model;

class PricingEngine {
  public:
    virtual ~PricingEngine() = default;
    virtual void set_model(std::shared_ptr<Model> model) = 0;
    virtual void set_market_data(std::shared_ptr<MarketData> market_data) = 0;
    virtual double compute(const Instrument& instrument) const = 0;
};

#endif

