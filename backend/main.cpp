#include <Eigen/Dense>
#include <iostream>
#include <iomanip>
#include <string>
#include "core/MarketData.hpp"
#include "engines/BinomialTreeEngine.hpp"
#include "engines/MonteCarloEngine.hpp"
#include "enums/TreeModel.hpp"
#include "utils/UsualPayoffs.hpp"
#include "models/BlackScholeModel.hpp"
#include "structs/BasisFunctions.hpp"
#include "rest_api/server.hpp"

int main(){

  int n_sim = 1000;
  int n_steps = 1000;
  BasisFunctions basis_func = make_polynomial_basis(3);
  
  MonteCarloEngine mc_engine(n_sim, n_steps);
  BinomialTreeEngine bt_engine(n_steps, TreeModel::JarrowRudd);
  std::shared_ptr<MarketData> market_data = std::make_shared<MarketData>();
  std::shared_ptr<BlackScholesModel> bs_model = std::make_shared<BlackScholesModel>();

  double S0 = 100;
  double K = 100;
  double sigma = 0.2;
  double r = 0.02;
  double t = 0;
  double T = 1;

  ExerciseKind exercise_kind = ExerciseKind::American;
  
  bs_model->set_r(r);
  bs_model->set_sigma(sigma);
  bs_model->set_spot(S0);

  mc_engine.set_model(bs_model);
  mc_engine.set_market_data(market_data);
  mc_engine.set_basis_functions(basis_func);

  bt_engine.set_model(bs_model);
  bt_engine.set_market_data(market_data);

  Option put(t, t, T, K,put_payoff, exercise_kind, false);

  std::cout << mc_engine.get_n_sim() << std::endl;
  std::cout << mc_engine.get_n_steps() << std::endl;

  OptionPriceResult mc_result = mc_engine.compute_price(put);
  double bt_price = bt_engine.compute_price(put);
  
  if (mc_result.confidence_interval) {
    auto& [low_estimator, high_estimator] = *mc_result.confidence_interval;
    std::cout << low_estimator << " <= " << mc_result.price 
              << " <= " << high_estimator << std::endl;
  }

  std::cout << bt_price << std::endl;

}
