#ifndef BINOMIALTREEENGINE_HPP
#define BINOMIALTREEENGINE_HPP

#include <memory>
#include "core/Instrument.hpp"
#include "core/PricingEngine.hpp"
#include "core/UnderlyingModel.hpp"
#include "instruments/equity/Option.hpp"
#include "enums/TreeModel.hpp"

class BinomialTreeEngine : public PricingEngine{
  public:
    BinomialTreeEngine() = default;
    BinomialTreeEngine(int n_steps, TreeModel tree_model);
    BinomialTreeEngine(std::shared_ptr<MarketData> market_data);
    BinomialTreeEngine(std::shared_ptr<MarketData> market_data, std::shared_ptr<UnderlyingModel> underlying_model);
    
    double compute_price(const Option& option) const;
    
    void set_n_steps(int n_step);
    void set_tree_model(TreeModel tree_model);   

    int get_n_steps() const;
    TreeModel get_tree_model() const;

    std::tuple<double,double> generate_proba_tree(double T) const;
    std::tuple<double,double> generate_up_down_tree(double T) const;
  private:
    int n_steps;
    TreeModel tree_model; 

};

#endif
