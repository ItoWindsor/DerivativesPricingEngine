#include <chrono>
#include <unordered_map>
#include "engines/AnalyticalEngine.hpp"
#include "core/MarketData.hpp"
#include "enums/CompoundingConvention.hpp"
#include "enums/DayCountConvention.hpp"
#include "models/BlackScholeModel.hpp"
#include "utils/DayCount.hpp"
#include "utils/CurveInterpolation.hpp"
#include "instruments/equity/CallOption.hpp"
#include "enums/ExerciseConvention.hpp"
#include "enums/ModelNames.hpp"
#include "utils/AnalyticalFormulas.hpp"


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
