#ifndef OPTIONPRICERESULT_HPP
#define OPTIONPRICERESULT_HPP

#include <optional>
#include <tuple>

struct OptionPriceResult {
    double price;
    std::optional<std::tuple<double, double>> confidence_interval;

    OptionPriceResult(
      double p,
      std::optional<std::tuple<double, double>> ci = std::nullopt)
      : price(p), confidence_interval(ci) {}
};

#endif
