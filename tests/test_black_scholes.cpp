#include <gtest/gtest.h>
#include "models/BlackScholeModel.hpp"
#include "engines/AnalyticalEngine.hpp"

TEST(BlackScholesTest, temp){
  EXPECT_EQ(1+1,2);
}

//TEST(BlackScholesTest, CallAnalyticalPricing){
//  double spot = 100.0;
//  double sigma = 0.2;
//  double r = 0.04;
//
//  BlackScholesModel model = BlackScholesModel(spot,sigma,r);
//  AnalyticalEngine engine;
//
//}
