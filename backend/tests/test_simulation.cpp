#include <gtest/gtest.h>
#include "utils/Simulation.hpp"

const int big_n_sim = 1000000;
TEST(GaussianVectorTest, GaussianVectorStats) {
  const double mean = 1.0;
  const double stdd = 2.0;
  
  Eigen::VectorXd vec = simulate_standard_vector_gaussian(big_n_sim, mean, stdd);
  
  double sample_mean = vec.mean();
  double sample_var = (vec.array() - sample_mean).square().sum() / (big_n_sim - 1);
  
  EXPECT_NEAR(sample_mean, mean, 0.1);
  EXPECT_NEAR(sample_var, stdd * stdd, 0.1);
}

TEST(BrownianMotionTest, PathDimensions) {
    const double T = 1.0;
    const int n_time = 100;
    const int n_sim = 5;
    
    Eigen::MatrixXd paths = simulate_brownian_paths(T, n_time, n_sim);
    
    EXPECT_EQ(paths.rows(), n_sim);
    EXPECT_EQ(paths.cols(), n_time + 1);
    EXPECT_TRUE(paths.col(0).isZero());
}

TEST(BrownianMotionTest, TerminalDistribution) {
    const double T = 2.0;
    
    Eigen::VectorXd terminal = simulate_brownian_terminal_distribution(T, big_n_sim);
    
    double sample_mean = terminal.mean();
    double sample_var = (terminal.array() - sample_mean).square().sum() / (big_n_sim - 1);
    EXPECT_NEAR(sample_mean, 0.0, 0.1);
    EXPECT_NEAR(sample_var, T, 0.1);
}

