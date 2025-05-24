#include <algorithm>
#include "utils/UsualPayoffs.hpp"

double call_payoff(double ST, double K){
  return std::max(ST-K,0.0);
}

double put_payoff(double ST, double K){
  return std::max(K-ST,0.0);
}
