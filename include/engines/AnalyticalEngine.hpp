#ifndef ANALYTICALENGINE_HPP
#define ANALYTICALENGINE_HPP

#include <memory>
#include <unordered_map>
#include "core/MarketData.hpp"
#include "core/Instrument.hpp"
#include "core/UnderlyingModel.hpp"
#include "instruments/Bond.hpp"
#include "instruments/equity/CallOption.hpp"

class AnalyticalEngine {
  
  public:
    AnalyticalEngine() = default;
    AnalyticalEngine(std::shared_ptr<MarketData> market_data);
    AnalyticalEngine(std::shared_ptr<MarketData> market_date, UnderlyingModel underlying_model);
    
    void set_model(std::shared_ptr<UnderlyingModel> model);
    void set_market_data(std::shared_ptr<MarketData> market_data);
    double compute_price(const Bond& bond);
    double compute_price(const CallOption& call);
    std::unordered_map<std::string, double> compute_sensitiviies(const CallOption& call);
  private:
    std::shared_ptr<MarketData> market_data;
    std::shared_ptr<UnderlyingModel> model;
};

#endif
