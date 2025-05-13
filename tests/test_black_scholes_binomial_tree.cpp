#include <gtest/gtest.h>
#include "models/BlackScholeModel.hpp"
#include "engines/BinomialTreeEngine.hpp"
#include "engines/AnalyticalEngine.hpp"
#include "instruments/equity/CallOption.hpp"
#include "enums/TreeModel.hpp"


TEST(BinomialTreeEngine, BlackScholesCallPricing_CRR){
  int n_steps = 1000;
  TreeModel tree_model = TreeModel::CoxRossRubinstein;
    
  AnalyticalEngine analytical_engine;
  BinomialTreeEngine engine(n_steps, tree_model);
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

  double price = engine.compute_price(call);
  double expected_price = analytical_engine.compute_price(call); 
  
  
  EXPECT_NEAR(price, expected_price, 0.1); 
}


TEST(BinomialTreeEngine, BlackScholesCallPricing_JR){
  int n_steps = 1000;
  TreeModel tree_model = TreeModel::JarrowRudd;
    
  AnalyticalEngine analytical_engine;
  BinomialTreeEngine engine(n_steps, tree_model);
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

  double price = engine.compute_price(call);
  double expected_price = analytical_engine.compute_price(call); 
  
  
  EXPECT_NEAR(price, expected_price, 0.1); 
}
