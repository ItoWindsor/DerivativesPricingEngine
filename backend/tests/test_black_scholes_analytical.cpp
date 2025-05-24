#include <gtest/gtest.h>
#include "utils/AnalyticalFormulas.hpp"

const double EPSILON = 1e-4;

TEST(BlackScholesTest, CallPrice) {
  double S = 100.0;
  double K = 100.0;
  double tau = 1.0;
  double sigma = 0.2;
  double r = 0.05;

  double price = compute_bs_call_price(S, K, tau, sigma, r);
  EXPECT_NEAR(price, 10.4506, EPSILON);
}

TEST(BlackScholesTest, PutPrice) {
  double S = 100.0;
  double K = 100.0;
  double tau = 1.0;
  double sigma = 0.2;
  double r = 0.05;

  double price = compute_bs_put_price(S, K, tau, sigma, r);
  EXPECT_NEAR(price, 5.5735, EPSILON);
}

TEST(BlackScholesTest, CallDelta) {
  double S = 100.0;
  double K = 100.0;
  double tau = 1.0;
  double sigma = 0.2;
  double r = 0.05;

  double delta = compute_bs_call_delta(S, K, tau, sigma, r);
  EXPECT_NEAR(delta, 0.6368, EPSILON);
}

TEST(BlackScholesTest, CallVega) {
  double S = 100.0;
  double K = 100.0;
  double tau = 1.0;
  double sigma = 0.2;
  double r = 0.05;

  double vega = compute_bs_call_vega(S, K, tau, sigma, r);
  EXPECT_NEAR(vega, 37.5240, EPSILON);
}

TEST(BlackScholesTest, CallTheta) {
  double S = 100.0;
  double K = 100.0;
  double tau = 1.0;
  double sigma = 0.2;
  double r = 0.05;

  double theta = compute_bs_call_theta(S, K, tau, sigma, r);
  EXPECT_NEAR(theta, -6.4140, EPSILON);
}

TEST(BlackScholesTest, CallGamma) {
  double S = 100.0;
  double K = 100.0;
  double tau = 1.0;
  double sigma = 0.2;
  double r = 0.05;

  double gamma = compute_bs_call_gamma(S, K, tau, sigma, r);
  EXPECT_NEAR(gamma, 0.0188, EPSILON);
}
