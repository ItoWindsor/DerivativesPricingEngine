#include "engines/BinomialTreeEngine.hpp"
#include "core/UnderlyingModel.hpp"
#include "enums/DayCountConvention.hpp"
#include "enums/ModelNames.hpp"
#include "utils/DayCount.hpp"
#include "models/BlackScholeModel.hpp"
#include "enums/TreeModel.hpp"
#include <tuple>

BinomialTreeEngine::BinomialTreeEngine(std::shared_ptr<MarketData> market_data)
  : PricingEngine(market_data) {}

BinomialTreeEngine::BinomialTreeEngine(std::shared_ptr<MarketData> market_data, std::shared_ptr<UnderlyingModel> underlying_model)
  : PricingEngine(market_data,underlying_model) {}

BinomialTreeEngine::BinomialTreeEngine(int n_step, TreeModel tree_model)
: n_steps(n_step), tree_model(tree_model) {}

int BinomialTreeEngine::get_n_steps() const{
  return this->n_steps;
}

TreeModel BinomialTreeEngine::get_tree_model() const{
  return this->tree_model;
}

std::tuple<double,double> BinomialTreeEngine::generate_proba_tree() const{
  
  double pu = 0.0;
  double pd = 0.0;

  switch (this->model->get_model_name()){
    
    case ModelName::BlackScholes : {
      
      switch (this->get_tree_model()) {
        case TreeModel::CoxRossRubinstein : {
          // TODO 
          auto bs = std::static_pointer_cast<BlackScholesModel>(model);
          double sigma = bs->get_sigma();
          return std::make_tuple(pu,pd);
        }
        case TreeModel::JarrowRudd : {
          pu = 0.5;
          pd = 0.5;
          return std::make_tuple(pu,pd);
        }
      }
    }

    case ModelName::Heston : {
      return std::make_tuple(pu,pd);
    }

  }

}
