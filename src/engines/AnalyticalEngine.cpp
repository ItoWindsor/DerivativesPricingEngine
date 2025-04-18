#include <chrono>
#include "engines/AnalyticalEngine.hpp"
#include "core/MarketData.hpp"


AnalyticalEngine::AnalyticalEngine(std::shared_ptr<MarketData> market_data)
    : market_data(std::move(market_data)) {}

double AnalyticalEngine::compute_price(const Bond& bond){
  auto curve = market_data->get_rate_curve();
  std::chrono::sys_days valuation_date = bond.get_valuation_date();
  std::chrono::sys_days maturity_date = bond.get_maturity_date();

  double price = 0.0;
  return price;
}
