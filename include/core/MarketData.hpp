#ifndef MARKETDATA_HPP
#define MARKETDATA_HPP

#include "core/InterestRateCurve.hpp"

class MarketData {
  public : 
    void load_interest_rate_curve(const std::string& filepath);
    std::shared_ptr<InterestRateCurve> get_rate_curve() const;
  private:
    std::shared_ptr<InterestRateCurve> interestRateCurve;
};

#endif
