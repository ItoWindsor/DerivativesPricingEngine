#include <memory>
#include "core/MarketData.hpp"
#include "core/PricingEngine.hpp"

PricingEngine::PricingEngine(std::shared_ptr<MarketData> market_data)
    : market_data(std::move(market_data)) {}


PricingEngine::PricingEngine(std::shared_ptr<MarketData> market_data, std::shared_ptr<UnderlyingModel> underlying_model)
    : market_data(std::move(market_data)), model(std::move(underlying_model)) {}

void PricingEngine::set_model(std::shared_ptr<UnderlyingModel> ptr_model) {
    this->model = std::move(ptr_model);
}

void PricingEngine::set_market_data(std::shared_ptr<MarketData> ptr_market_data) {
    this->market_data = std::move(ptr_market_data);
}
