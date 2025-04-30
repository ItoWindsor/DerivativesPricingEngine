#ifndef SIMULATION_HPP
#define SIMULATION_HPP

#include <eigen/Dense>

Eigen::VectorXd simulate_standard_vector_gaussian(int n_sim, double mean = 0.0, double stdd = 0.0);
Eigen::VectorXd simulate_brownian_terminal_distribution(double T, int n_sim);
Eigen::MatrixXd simulate_brownian_paths(double T, int n_time,int n_sim);

#endif
