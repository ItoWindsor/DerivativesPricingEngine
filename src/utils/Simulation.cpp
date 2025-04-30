#include <random>
#include <Eigen/Dense>
#include <numeric>
#include "utils/Simulation.hpp"


Eigen::VectorXd simulate_standard_vector_gaussian(int n_sim, double mean, double stdd){
  std::random_device rd;
  std::mt19937 gen(rd());
  
  Eigen::VectorXd gaussian_vec = Eigen::VectorXd::NullaryExpr(n_sim, [&]() {
        std::normal_distribution<double> dist(mean, stdd); 
        return dist(gen);
    });

  return gaussian_vec;
}

Eigen::VectorXd simulate_brownian_terminal_distribution(double T, int n_sim){
  Eigen::VectorXd vec = simulate_standard_vector_gaussian(n_sim,0.0, std::sqrt(T)); 
  return vec; 
}

Eigen::MatrixXd simulate_brownian_paths(double T, int n_time, int n_sim) {
    std::random_device rd;
    std::mt19937 gen(rd());
    
    double h = T/n_time;  
    
    Eigen::MatrixXd noise = Eigen::MatrixXd::NullaryExpr(n_sim, n_time, [&]() {
        std::normal_distribution<double> dist(0.0, std::sqrt(h));
        return dist(gen);
    });

    Eigen::MatrixXd paths(n_sim, n_time + 1);  
    paths.col(0).setZero();
    
    for (int i = 0; i < n_sim; ++i) {
        std::partial_sum(noise.row(i).begin(), noise.row(i).end(), paths.row(i).begin() + 1);
    }
    
    return paths;
}
