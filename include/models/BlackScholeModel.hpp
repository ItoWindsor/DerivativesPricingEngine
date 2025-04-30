#ifndef BLACKSCHOLESMODEL_HPP
#define BLACKSCHOLESMODEL_HPP

#include "core/UnderlyingModel.hpp"
#include "core/MarketData.hpp"

class BlackScholesModel : public UnderlyingModel {
  public:
    BlackScholesModel(double spot = 100.0, double sigma = 0.2, double r = 0.1);
    void fit(const MarketData& market_data) override;
    double get_spot() const;
    double get_sigma() const;
    double get_interest_rate() const;
    void set_spot();
    void set_sigma();
    void set_r();
  private:
    double spot;
    double sigma;
    double r;
};

#endif
