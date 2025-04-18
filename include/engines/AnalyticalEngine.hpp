#ifndef ANALYTICALENGINE_HPP
#define ANALYTICALENGINE_HPP

#include <memory>
#include "core/MarketData.hpp"
#include "core/Instrument.hpp"
#include "instruments/bond.hpp"

class AnalyticalEngine {
  
  public:
    AnalyticalEngine(std::shared_ptr<MarketData> market_data);
    double compute_price(const Bond& bond);
    double compute_sensitiviies(const Instrument& instrument);
  private:
    std::shared_ptr<MarketData> market_data;
};

#endif
