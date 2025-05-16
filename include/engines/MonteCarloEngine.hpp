#ifndef MONTECARLOENGINE_HPP
#define MONTECARLOENGINE_HPP

#include <memory>
#include <unordered_map>
#include "core/Instrument.hpp"
#include "core/PricingEngine.hpp"
#include "core/UnderlyingModel.hpp"
#include "instruments/equity/Option.hpp"
#include "structs/OptionPriceResult.hpp"
#include "structs/BasisFunctions.hpp"

class MonteCarloEngine : public PricingEngine{
  public:
    MonteCarloEngine() = default;
    MonteCarloEngine(int n_sim, int n_step = 1);
    MonteCarloEngine(std::shared_ptr<MarketData> market_data);
    MonteCarloEngine(std::shared_ptr<MarketData> market_data, std::shared_ptr<UnderlyingModel> underlying_model);
    OptionPriceResult compute_price(const Option& option) const;
    void set_n_simulation(int n_sim);
    void set_n_steps(int n_step);
    void set_basis_functions(const BasisFunctions& basis);

    int get_n_sim() const;
    int get_n_steps() const;
    const BasisFunctions& get_basis_functions() const;
  private:
    int n_simulation;
    int n_steps;
    BasisFunctions basis_functions; 
};

#endif
