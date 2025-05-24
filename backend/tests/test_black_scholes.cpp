#include <gtest/gtest.h>
#include "models/BlackScholeModel.hpp"
#include "engines/AnalyticalEngine.hpp"

TEST(BlackScholesCall, CallAnalyticalPricing){
  AnalyticalEngine engine;
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

  CallOption call = CallOption(t,t,T,K,exercise_kind, false);

  double price = engine.compute_price(call);
  double expected_price = 8.916;
  
  EXPECT_NEAR(price, expected_price, 1e-3);
}


TEST(BlackScholesCall, DeepITMCallOptionPrice){
  AnalyticalEngine engine;
  auto market_data = std::make_shared<MarketData>();
  auto bs_model = std::make_shared<BlackScholesModel>();

  double S0 = 150;
  double K = 100;
  double sigma = 0.2;
  double r = 0.02;
  double t = 0;
  double T = 1;
  ExerciseKind kind = ExerciseKind::European;

  bs_model->set_spot(S0);
  bs_model->set_sigma(sigma);
  bs_model->set_r(r);

  engine.set_model(bs_model);
  engine.set_market_data(market_data);

  CallOption call(t, t, T, K, kind, false);
  double price = engine.compute_price(call);

  double expected_price = 52.125; 

  EXPECT_NEAR(price, expected_price, 1e-3);
}

TEST(BlackScholesTest, CallPriceIncreasesWithSpot){
  AnalyticalEngine engine;
  auto market_data = std::make_shared<MarketData>();
  auto bs_model = std::make_shared<BlackScholesModel>();

  double K = 100;
  double sigma = 0.2;
  double r = 0.02;
  double t = 0;
  double T = 1;
  ExerciseKind kind = ExerciseKind::European;

  bs_model->set_sigma(sigma);
  bs_model->set_r(r);
  engine.set_model(bs_model);
  engine.set_market_data(market_data);

  CallOption call(t, t, T, K, kind, false);

  bs_model->set_spot(90);
  double price1 = engine.compute_price(call);

  bs_model->set_spot(100);
  double price2 = engine.compute_price(call);

  bs_model->set_spot(110);
  double price3 = engine.compute_price(call);

  EXPECT_LT(price1, price2);
  EXPECT_LT(price2, price3);
}

TEST(BlackScholesCall, DeepOTMCallOptionPrice){
  AnalyticalEngine engine;
  auto market_data = std::make_shared<MarketData>();
  auto bs_model = std::make_shared<BlackScholesModel>();

  double S0 = 70;
  double K = 100;
  double sigma = 0.2;
  double r = 0.02;
  double t = 0;
  double T = 1;
  ExerciseKind kind = ExerciseKind::European;

  bs_model->set_spot(S0);
  bs_model->set_sigma(sigma);
  bs_model->set_r(r);

  engine.set_model(bs_model);
  engine.set_market_data(market_data);

  CallOption call(t, t, T, K, kind, false);
  double price = engine.compute_price(call);

  double expected_price = 0.314;  

  EXPECT_NEAR(price, expected_price, 1e-3);
}
