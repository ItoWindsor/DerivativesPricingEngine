#include <gtest/gtest.h>
#include "models/BlackScholeModel.hpp"
#include "engines/MonteCarloEngine.hpp"
#include "engines/AnalyticalEngine.hpp"
#include "instruments/equity/CallOption.hpp"

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
