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

std::tuple<double,double> BinomialTreeEngine::generate_proba_tree(double T) const{
  
  double pu = 0.0;
  double pd = 0.0;
  double step = T/(this->get_n_steps());
  
  switch (this->model->get_model_name()){
    
    case ModelName::BlackScholes : {
      auto bs = std::static_pointer_cast<BlackScholesModel>(model);
      switch (this->get_tree_model()) {
        case TreeModel::CoxRossRubinstein : {
          // TODO 
          std::tuple<double,double> probas = this->generate_up_down_tree(T);
          double u = std::get<0>(probas);
          double d = std::get<1>(probas);
          double r = bs->get_interest_rate();

          pu =( std::exp(r*std::sqrt(step) - d))/(u-d);
          pd = 1.0 - pu;
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

//TODO 
std::tuple<double,double> BinomialTreeEngine::generate_up_down_tree(double T) const{
  double u = 0.0;
  double d = 0.0;
  double step = T/(this->get_n_steps());

  switch(this->model->get_model_name()){

    case ModelName::BlackScholes : {
      auto bs = std::static_pointer_cast<BlackScholesModel>(model);
      double sigma = bs->get_sigma();
      switch (this->get_tree_model()){

        case TreeModel::CoxRossRubinstein : {
          u = std::exp(sigma * std::sqrt(step));
          d = 1/u;
          return std::make_tuple(u,d);
        }

        case TreeModel::JarrowRudd : {
          double r = bs->get_interest_rate();
          // double q = ... TODO
          u = std::exp( (r - 0.5*sigma*sigma)*step + sigma*std::sqrt(step) );
          d = std::exp( (r - 0.5*sigma*sigma)*step - sigma*std::sqrt(step) );
          return std::make_tuple(u,d);
        }
      }

    }

    case ModelName::Heston : {
        return std::make_tuple(u,d);
      }
  }
}
