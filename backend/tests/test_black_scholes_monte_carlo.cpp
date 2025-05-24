#include <gtest/gtest.h>
#include "models/BlackScholeModel.hpp"
#include "engines/MonteCarloEngine.hpp"
#include "engines/AnalyticalEngine.hpp"
#include "engines/BinomialTreeEngine.hpp"
#include "instruments/equity/CallOption.hpp"
#include "utils/UsualPayoffs.hpp"
#include "enums/TreeModel.hpp"
#include "structs/BasisFunctions.hpp"

TEST(MonteCarlo, BlackScholesCallPricing){
  int n_sim = 100000; // large enough for accuracy
  int n_steps = 1;

  AnalyticalEngine analytical_engine;
  MonteCarloEngine engine(n_sim, n_steps);
  std::shared_ptr<MarketData> market_data = std::make_shared<MarketData>();
  std::shared_ptr<BlackScholesModel> bs_model = std::make_shared<BlackScholesModel>();

  double S0 = 100;
  double K = 100;
  double sigma = 0.2;
  double r = 0.02;
  double t = 0;
  double T = 1;

  ExerciseKind exercise_kind = ExerciseKind::European;

  bs_model->set_r(r);
  bs_model->set_sigma(sigma);
  bs_model->set_spot(S0);

  engine.set_model(bs_model);
  engine.set_market_data(market_data);

  analytical_engine.set_model(bs_model);
  analytical_engine.set_market_data(market_data);
  
  CallOption call = CallOption(t, t, T, K, exercise_kind, false);

  OptionPriceResult result = engine.compute_price(call);
  double expected_price = analytical_engine.compute_price(call); 
  
  
  EXPECT_NEAR(result.price, expected_price, 0.1); 
}


TEST(MonteCarlo, AmericanPut_MatchesBinomialTree) {
  int n_sim = 1000;
  int n_steps = 100;
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

  OptionPriceResult mc_result = mc_engine.compute_price(put);
  double bt_price = bt_engine.compute_price(put);

  EXPECT_NEAR(mc_result.price, bt_price, 0.5); // Slightly looser tolerance due to MC noise
}
