#ifndef SIMULATION_HPP
#define SIMULATION_HPP

#include <eigen/Dense>

Eigen::VectorXd simulate_standard_vector_gaussian(int n_sim, double mean = 0.0, double stdd = 0.0);
Eigen::VectorXd simulate_brownian_terminal_distribution(double T, int n_sim);
Eigen::MatrixXd simulate_brownian_paths(double T, int n_time,int n_sim);
Eigen::VectorXd simulate_geometrical_brownian_terminal_distribution(double S, double T, double r, double sigma, int n_sim);
Eigen::MatrixXd simulate_geometrical_brownian_paths(double S, double T, double r, double sigma, int n_sim, int n_time);
std::tuple<double,double> compute_confidence_interval(Eigen::VectorXd results, double z_score = 1.96);

#endif
