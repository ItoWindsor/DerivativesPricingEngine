#ifndef ANALYTICALENGINE_HPP
#define ANALYTICALENGINE_HPP

#include <memory>
#include <unordered_map>
#include "core/PricingEngine.hpp"
#include "core/MarketData.hpp"
#include "core/Instrument.hpp"
#include "core/UnderlyingModel.hpp"
#include "instruments/interest_rates/Bond.hpp"
#include "instruments/equity/CallOption.hpp"

class AnalyticalEngine : public PricingEngine{
  
  public:
    AnalyticalEngine() = default;
    AnalyticalEngine(std::shared_ptr<MarketData> market_data);
    AnalyticalEngine(std::shared_ptr<MarketData> market_data, std::shared_ptr<UnderlyingModel> underlying_model);
    double compute_price(const Bond& bond) const;
    double compute_price(const CallOption& call) const;
    std::unordered_map<std::string, double> compute_sensitiviies(const CallOption& call);
};

#endif
