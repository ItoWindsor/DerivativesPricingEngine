#include "models/BlackScholeModel.hpp"
#include "core/MarketData.hpp"
#include "core/UnderlyingModel.hpp"
#include "enums/ModelNames.hpp"

BlackScholesModel::BlackScholesModel(double spot_, double sigma_, double r_)
  : UnderlyingModel(ModelName::BlackScholes), spot(spot_), sigma(sigma_), r(r_) {}

void BlackScholesModel::fit(const MarketData& market_data,const std::string& asset_name){
  double sigma_ = 0.2;
  double r_ = 0.05;
  double spot_ = market_data.get_stock_price(asset_name);
  this->set_sigma(sigma_);
  this->set_spot(spot_);
  this->set_r(r_);
}

void BlackScholesModel::set_r(double r_){
  this->r = r_;
};

void BlackScholesModel::set_spot(double spot_){ 
  this->spot = spot_;
};


void BlackScholesModel::set_sigma(double sigma_){
  this->sigma = sigma_;
};


double BlackScholesModel::get_interest_rate() const{
  return this->r;
};

double BlackScholesModel::get_spot() const{
  return this->spot;
};

double BlackScholesModel::get_sigma() const{
  return this->sigma;
};
