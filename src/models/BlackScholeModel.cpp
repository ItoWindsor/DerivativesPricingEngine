#include "models/BlackScholeModel.hpp"
#include "core/MarketData.hpp"
#include "core/UnderlyingModel.hpp"
#include "enums/ModelNames.hpp"

BlackScholesModel::BlackScholesModel(double spot, double sigma, double r)
  : UnderlyingModel(ModelName::BlackScholes), spot(spot), sigma(sigma), r(r) {}

void BlackScholesModel::fit(const MarketData& market_data){
  this->r = 0.2;
  this->set_sigma();
  this->set_spot();
  this->set_r();
}

void BlackScholesModel::set_r(){

};

void BlackScholesModel::set_spot(){

};


void BlackScholesModel::set_sigma(){

};


