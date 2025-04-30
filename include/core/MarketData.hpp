#ifndef MARKETDATA_HPP
#define MARKETDATA_HPP

#include "core/InterestRateCurve.hpp"
#include <unordered_map>

class MarketData {
  public :
    MarketData() = default;
    void load_interest_rate_curve(const std::string& filepath, std::chrono::sys_days valuation_date);
    void load_stock_prices(const std::string& filepath);
    void load_stock_prices(const std::vector<std::tuple<std::string, double>>& stock_prices);
    std::shared_ptr<InterestRateCurve> get_rate_curve() const;
    std::unordered_map<std::string, double> get_stock_prices() const;
    double get_stock_price(const std::string& stock_name) const;
  
  private:
    std::shared_ptr<InterestRateCurve> interestRateCurve;
    std::unordered_map<std::string, double> stockPrices;
};

#endif
