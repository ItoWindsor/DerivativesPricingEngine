#ifndef ANALYTICALFORMULAS_HPP
#define ANALYTICALFORMULAS_HPP

double compute_bs_call_price(double S, double K, double tau, double sigma, double r);
double compute_bs_call_delta(double S, double K, double tau, double sigma, double r);
double compute_bs_call_vega(double S, double K, double tau, double sigma, double r);
double compute_bs_call_theta(double S, double K, double tau, double sigma, double r);
double compute_bs_call_gamma(double S, double K, double tau, double sigma, double r);

double compute_bs_put_price(double S, double K, double tau, double sigma, double r);

#endif
