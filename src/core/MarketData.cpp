#include "core/MarketData.hpp"
#include "core/InterestRateCurve.hpp"

std::shared_ptr<InterestRateCurve> MarketData::get_rate_curve() const{
  return this->interestRateCurve;
}


void MarketData::load_interest_rate_curve(const std::string& filepath, std::chrono::sys_days valuation_date) {
    interestRateCurve = std::make_shared<InterestRateCurve>(filepath, valuation_date);
}

void MarketData::load_stock_prices(const std::string& filepath){

}

void MarketData::load_stock_prices(const std::vector<std::tuple<std::string, double>>& stock_prices) {
  for (auto& [name, price] : stock_prices) {
    this->stockPrices[std::move(name)] = price;
  }
}

std::unordered_map<std::string, double> MarketData::get_stock_prices() const{
  return this->stockPrices;
}
  
double MarketData::get_stock_price(const std::string& stock_name) const{
  return this->stockPrices.at(stock_name);  
}
