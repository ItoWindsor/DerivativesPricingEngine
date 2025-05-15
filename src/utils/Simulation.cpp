#include <random>
#include <Eigen/Dense>
#include <numeric>
#include <cmath>
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

Eigen::VectorXd simulate_geometrical_brownian_terminal_distribution(double S, double T, double r, double sigma, int n_sim){
  auto  vec_random = simulate_brownian_terminal_distribution(T,n_sim);
  for(auto i = 0 ; i < vec_random.size(); ++i){
    vec_random[i] = S*std::exp( std::pow(r - (0.5 * sigma*sigma),2)*std::sqrt(T) + sigma*vec_random[i]); 
  }
  return vec_random;
}

Eigen::MatrixXd simulate_geometrical_brownian_paths(double S, double T, double r, double sigma, int n_sim, int n_time) {
    Eigen::MatrixXd brownian = simulate_brownian_paths(T, n_time, n_sim);  

    Eigen::VectorXd t = Eigen::VectorXd::LinSpaced(n_time + 1, 0.0, T);

    Eigen::VectorXd drift = (r - 0.5 * sigma * sigma) * t;

    Eigen::MatrixXd stock_mat = (drift.transpose().replicate(n_sim, 1) + sigma * brownian).array().exp();
    
    return S * stock_mat.array();  
}

std::tuple<double, double> compute_confidence_interval(Eigen::VectorXd results, double z_score) {
  
  double mean = results.mean();

  Eigen::ArrayXd squared_diffs = (results.array() - mean).square();

  double std_dev = std::sqrt(squared_diffs.sum() / static_cast<double>(results.size()));
  double std_error = std_dev / std::sqrt(static_cast<double>(results.size()));
  
  double lower_bound = mean - z_score * std_error;
  double upper_bound = mean + z_score * std_error;

  return std::make_tuple(lower_bound, upper_bound);
}
