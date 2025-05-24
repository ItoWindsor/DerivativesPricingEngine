#include <cmath>
#include "utils/AnalyticalFormulas.hpp"

double compute_bs_call_price(double S, double K, double tau, double sigma, double r) {
  double d1 = (std::log(S / K) + (r + 0.5 * sigma * sigma) * tau) / (sigma * std::sqrt(tau));
  double d2 = d1 - sigma * std::sqrt(tau);

  double Nd1 = 0.5 * (1 + std::erf(d1 / std::sqrt(2)));
  double Nd2 = 0.5 * (1 + std::erf(d2 / std::sqrt(2)));

  return S * Nd1 - K * std::exp(-r * tau) * Nd2;
}

double compute_bs_call_delta(double S, double K, double tau, double sigma, double r) {
  double d1 = (std::log(S / K) + (r + 0.5 * sigma * sigma) * tau) / (sigma * std::sqrt(tau));
  return 0.5 * (1 + std::erf(d1 / std::sqrt(2)));
}

double compute_bs_call_vega(double S, double K, double tau, double sigma, double r) {
  double d1 = (std::log(S / K) + (r + 0.5 * sigma * sigma) * tau) / (sigma * std::sqrt(tau));
  double pdf_d1 = (1.0 / std::sqrt(2 * M_PI)) * std::exp(-0.5 * d1 * d1);
  return S * pdf_d1 * std::sqrt(tau);
}

double compute_bs_call_theta(double S, double K, double tau, double sigma, double r) {
  double d1 = (std::log(S / K) + (r + 0.5 * sigma * sigma) * tau) / (sigma * std::sqrt(tau));
  double d2 = d1 - sigma * std::sqrt(tau);

  double pdf_d1 = (1.0 / std::sqrt(2 * M_PI)) * std::exp(-0.5 * d1 * d1);
  double Nd2 = 0.5 * (1 + std::erf(d2 / std::sqrt(2)));

  double term1 = - (S * pdf_d1 * sigma) / (2 * std::sqrt(tau));
  double term2 = r * K * std::exp(-r * tau) * Nd2;

  return term1 - term2;
}

double compute_bs_call_gamma(double S, double K, double tau, double sigma, double r) {
  double d1 = (std::log(S / K) + (r + 0.5 * sigma * sigma) * tau) / (sigma * std::sqrt(tau));
  double pdf_d1 = (1.0 / std::sqrt(2 * M_PI)) * std::exp(-0.5 * d1 * d1);
  return pdf_d1 / (S * sigma * std::sqrt(tau));
}

double compute_bs_put_price(double S, double K, double tau, double sigma, double r) {
  double d1 = (std::log(S / K) + (r + 0.5 * sigma * sigma) * tau) / (sigma * std::sqrt(tau));
  double d2 = d1 - sigma * std::sqrt(tau);

  double Nminus_d1 = 0.5 * (1 - std::erf(d1 / std::sqrt(2)));
  double Nminus_d2 = 0.5 * (1 - std::erf(d2 / std::sqrt(2)));

  return K * std::exp(-r * tau) * Nminus_d2 - S * Nminus_d1;
}
