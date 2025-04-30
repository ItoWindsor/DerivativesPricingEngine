#include "models/BlackScholeModel.hpp"
#include "core/MarketData.hpp"
#include "core/UnderlyingModel.hpp"
#include "enums/ModelNames.hpp"

BlackScholesModel::BlackScholesModel(double spot, double sigma, double r)
  : UnderlyingModel(ModelName::BlackScholes), spot(spot), sigma(sigma), r(r) {}

void BlackScholesModel::fit(const MarketData& market_data,const std::string& asset_name){
  double sigma = 0.2;
  double r = 0.05;
  double spot = market_data.get_stock_price(asset_name);
  this->set_sigma(sigma);
  this->set_spot(spot);
  this->set_r(r);
}

void BlackScholesModel::set_r(double r){
  this->r = r;
};

void BlackScholesModel::set_spot(double spot){ 
  this->spot = spot;
};


void BlackScholesModel::set_sigma(double sigma){
  this->sigma = sigma;
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
