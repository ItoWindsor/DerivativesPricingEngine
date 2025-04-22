#include "core/MarketData.hpp"

std::shared_ptr<InterestRateCurve> MarketData::get_rate_curve() const{
  return this->interestRateCurve;
}


void MarketData::load_interest_rate_curve(const std::string& filepath, std::chrono::sys_days valuation_date) {
    interestRateCurve = std::make_shared<InterestRateCurve>(filepath, valuation_date);
}
