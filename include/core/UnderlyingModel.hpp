#ifndef UNDERLYINGMODEL_HPP
#define UNDERLYINGMODEL_HPP

#include "core/MarketData.hpp"

class UnderlyingModel {
  public:
    virtual ~UnderlyingModel() = default;
    virtual void fit(const MarketData& market_data) = 0;
};

#endif
