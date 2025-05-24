#ifndef PRICINGENGINE_HPP
#define PRICINGENGINE_HPP

#include <memory>
#include "core/MarketData.hpp"
#include "core/Instrument.hpp"
#include "core/UnderlyingModel.hpp"

class PricingEngine {
  public:
    PricingEngine() = default;
    PricingEngine(std::shared_ptr<MarketData> market_data);
    PricingEngine(std::shared_ptr<MarketData> market_data, std::shared_ptr<UnderlyingModel> underlying_model);
    virtual ~PricingEngine() = default;
    void set_model(std::shared_ptr<UnderlyingModel> model);
    void set_market_data(std::shared_ptr<MarketData> market_data);
  protected:
    std::shared_ptr<MarketData> market_data;
    std::shared_ptr<UnderlyingModel> model;
};

#endif

