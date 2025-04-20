#include "core/InterestRateCurve.hpp"

const std::vector<TupleDateDouble>& InterestRateCurve::get_curve_data() const{
  return this->curve;
}
