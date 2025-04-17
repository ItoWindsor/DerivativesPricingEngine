#ifndef PRICINGENGINE_HPP
#define PRICINGENGINE_HPP

#include <memory>

class Instrument;
class Model;

class PricingEngine {
  public:
    virtual ~PricingEngine() = default;

    virtual void setModel(std::shared_ptr<Model> model) = 0;
    virtual double compute(const Instrument& instrument) const = 0;
};

#endif

