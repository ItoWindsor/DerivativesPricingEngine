#include "core/MarketData.hpp"

std::shared_ptr<InterestRateCurve> MarketData::get_rate_curve() const{
  return this->interestRateCurve;
}
