#include <gtest/gtest.h>
#include "models/BlackScholeModel.hpp"
#include "engines/BinomialTreeEngine.hpp"
#include "engines/AnalyticalEngine.hpp"
#include "instruments/equity/CallOption.hpp"
#include "enums/TreeModel.hpp"
#include "instruments/equity/Option.hpp"
#include "utils/UsualPayoffs.hpp"

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


TEST(BinomialTreeEngine, BlackScholes_Put_Pricing_CRR_American){
  int n_steps = 1000;
  TreeModel tree_model = TreeModel::CoxRossRubinstein;
  
  BinomialTreeEngine engine(n_steps, tree_model);
  std::shared_ptr<MarketData> market_data = std::make_shared<MarketData>();
  std::shared_ptr<BlackScholesModel> bs_model = std::make_shared<BlackScholesModel>();

  double K = 100;
  double sigma = 0.4;
  double r = 0.06;
  double t = 0;
  double T = 0.5;
  ExerciseKind exercise_kind = ExerciseKind::American;

  bs_model->set_r(r);
  bs_model->set_sigma(sigma);

  engine.set_model(bs_model);
  engine.set_market_data(market_data);

  std::unordered_map<double, double> americanOptionValues = {
      {80.0, 21.6059},
      {85.0, 18.0374},
      {90.0, 14.9187},
      {95.0, 12.2314},
      {100.0, 9.9458},
      {105.0, 8.0281},
      {110.0, 6.4352},
      {115.0, 5.1265},
      {120.0, 4.0611}
  };

  for (const auto& [S0, expected_price] : americanOptionValues) {
    bs_model->set_spot(S0);
    Option option(t, t, T, K, put_payoff, exercise_kind, false);
    double computed_price = engine.compute_price(option);
    EXPECT_NEAR(computed_price, expected_price, 0.01) << "Failed for S0 = " << S0;
  }
}


TEST(BinomialTreeEngine, BlackScholes_Put_Pricing_JR_American){
  int n_steps = 1000;
  TreeModel tree_model = TreeModel::JarrowRudd;
  
  BinomialTreeEngine engine(n_steps, tree_model);
  std::shared_ptr<MarketData> market_data = std::make_shared<MarketData>();
  std::shared_ptr<BlackScholesModel> bs_model = std::make_shared<BlackScholesModel>();

  double K = 100;
  double sigma = 0.4;
  double r = 0.06;
  double t = 0;
  double T = 0.5;
  ExerciseKind exercise_kind = ExerciseKind::American;

  bs_model->set_r(r);
  bs_model->set_sigma(sigma);

  engine.set_model(bs_model);
  engine.set_market_data(market_data);

  std::unordered_map<double, double> americanOptionValues = {
      {80.0, 21.6059},
      {85.0, 18.0374},
      {90.0, 14.9187},
      {95.0, 12.2314},
      {100.0, 9.9458},
      {105.0, 8.0281},
      {110.0, 6.4352},
      {115.0, 5.1265},
      {120.0, 4.0611}
  };

  for (const auto& [S0, expected_price] : americanOptionValues) {
    bs_model->set_spot(S0);
    Option option(t, t, T, K, put_payoff, exercise_kind, false);
    double computed_price = engine.compute_price(option);
    EXPECT_NEAR(computed_price, expected_price, 0.01) << "Failed for S0 = " << S0;
  }
}
