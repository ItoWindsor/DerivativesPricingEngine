#ifndef BLACKSCHOLESMODEL_HPP
#define BLACKSCHOLESMODEL_HPP

#include "core/UnderlyingModel.hpp"
#include "core/MarketData.hpp"

class BlackScholesModel : public UnderlyingModel {
  public:
    BlackScholesModel(double spot_ = 100.0, double sigma_ = 0.2, double r_ = 0.1);
    void fit(const MarketData& market_data, const std::string& asset_name) override;
    double get_spot() const;
    double get_sigma() const;
    double get_interest_rate() const;
    void set_spot(double spot_);
    void set_sigma(double sigma_);
    void set_r(double r_);
  private:
    double spot;
    double sigma;
    double r;
};

#endif
