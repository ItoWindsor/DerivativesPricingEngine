#ifndef MONTECARLOENGINE_HPP
#define MONTECARLOENGINE_HPP

#include <memory>
#include <unordered_map>
#include "core/Instrument.hpp"
#include "core/UnderlyingModel.hpp"
#include "instruments/Bond.hpp"
#include "instruments/equity/Option.hpp"
#include "structs/OptionPriceResult.hpp"

class MonteCarloEngine{
  public:
    MonteCarloEngine() = default;
    MonteCarloEngine(std::shared_ptr<MarketData> market_data);
    MonteCarloEngine(std::shared_ptr<MarketData> market_data, std::shared_ptr<UnderlyingModel> underlying_model);
    OptionPriceResult compute_price(const Option& option);
    void set_n_simulation(int n_sim);
    void set_n_steps(int n_step);
  private:
    int n_simulation;
    int n_steps;
};

#endif
