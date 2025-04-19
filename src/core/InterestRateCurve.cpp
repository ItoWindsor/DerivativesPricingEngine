#include "core/InterestRateCurve.hpp"

const std::vector<TupleDateDouble>& InterestRateCurve::get_curve() const{
  return this->curve;
}
