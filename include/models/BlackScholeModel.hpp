#ifndef BLACKSCHOLESMODEL_HPP
#define BLACKSCHOLESMODEL_HPP

#include "core/UnderlyingModel.hpp"
#include "core/MarketData.hpp"

class BlackScholesModel : public UnderlyingModel {
  public:
    BlackScholesModel() = default;
    BlackScholesModel(double spot, double sigma, double r);
    void fit(const MarketData& market_data) override;
  private:
    double spot;
    double sigma;
    double r;
};

#endif
