#include <iostream>
#include <fstream>
#include <sstream>
#include <stdexcept>
#include <chrono>
#include "enums/DayCountConvention.hpp"
#include "utils/CurveInterpolation.hpp"
#include "utils/DayCount.hpp"
#include "core/InterestRateCurve.hpp"

InterestRateCurve::InterestRateCurve(
  std::chrono::sys_days valuation_date,
  const std::vector<TupleDateDouble>& curve_data)
: valuationDate(valuation_date), curve(curve_data) {}

InterestRateCurve::InterestRateCurve(
  double valuation_time,
  const std::vector<std::tuple<double,double>>& curve_data)
: valuationDate(valuation_time), curve(curve_data) {}

InterestRateCurve::InterestRateCurve(
  const std::string& filepath, std::chrono::sys_days valuation_date)
  : valuationDate(valuation_date) {
  std::ifstream file(filepath);
  if (!file.is_open()){
    throw std::runtime_error("Could not open the file: " + filepath);
  }

  std::vector<TupleDateDouble> temp_curve;
  std::string line;
  bool is_header = true;

  while (std::getline(file, line)) {
    if (is_header){
      is_header = false;
      continue;
    }

    if (line.find_first_not_of(" \t\n\r") == std::string::npos) continue;

    std::stringstream streamed_string(line);
    std::string str_date;
    std::string str_rate;

    if (!std::getline(streamed_string, str_date, ',')) continue;
    if (!std::getline(streamed_string, str_rate, ',')) continue;

    // std::cout << "Parsed: " << str_date << ", Rate: " << str_rate << "\n";

    std::tm tm = {}; // std::chrono::parser() not available UNDER MACOS ZEBI
    std::istringstream stream_date(str_date);
    stream_date >> std::get_time(&tm, "%Y-%m-%d"); // expected format : YYYY-MM-DD
    if (stream_date.fail()){
      throw std::runtime_error("Could not parse the date : " + str_date);
    }

    std::chrono::sys_days date = std::chrono::year{static_cast<int>(tm.tm_year + 1900)} /
                                 std::chrono::month{static_cast<unsigned int>(tm.tm_mon + 1)} /
                                 std::chrono::day{static_cast<unsigned int>(tm.tm_mday)};
    double rate = std::stod(str_rate);
    temp_curve.emplace_back(date, rate);
  }

  curve = std::move(temp_curve);
}

InterestRateCurve::InterestRateCurve(
  const std::string& filepath, double valuation_time)
  : valuationDate(valuation_time)
{
  std::ifstream file(filepath);
  if (!file.is_open()){
    throw std::runtime_error("Could not open the file: " + filepath);
  }

  std::vector<std::tuple<double, double>> temp_curve;
  std::string line;
  bool is_header = true;

  while (std::getline(file, line)) {
    if (is_header){
      is_header = false;
      continue;
    }

    if (line.find_first_not_of(" \t\n\r") == std::string::npos) continue;

    std::stringstream streamed_string(line);
    std::string str_time;
    std::string str_rate;

    if (!std::getline(streamed_string, str_time, ',')) continue;
    if (!std::getline(streamed_string, str_rate, ',')) continue;

    // std::cout << "Parsed: " << str_time << ", Rate: " << str_rate << "\n";

    double time = std::stod(str_time);
    double rate = std::stod(str_rate);
    temp_curve.emplace_back(time, rate);
  }

  curve = std::move(temp_curve);
}

std::variant< std::vector<TupleDateDouble>, std::vector<std::tuple<double,double>> >  InterestRateCurve::get_curve_data() const{
  return this->curve;
}

const std::variant<std::chrono::sys_days,double> InterestRateCurve::get_valuation_date() const{
  return this->valuationDate;
}

std::vector<std::tuple<double, double>> InterestRateCurve::interpolate_curve(
    const std::vector<double>& curve_to_interpolate,
    DayCountConvention day_convention)
{
  std::vector<std::tuple<double, double>> base_curve;

  if (std::holds_alternative<std::vector<TupleDateDouble>>(curve) &&
      std::holds_alternative<std::chrono::sys_days>(valuationDate))
  {
    // Case with date-based curve
    const auto& base_curve_date = std::get<std::vector<TupleDateDouble>>(curve);
    const auto& val_date = std::get<std::chrono::sys_days>(valuationDate);

    base_curve = compute_year_fraction(val_date, base_curve_date, day_convention);
  }
  else if (std::holds_alternative<std::vector<std::tuple<double, double>>>(curve))
  {
    // Case with raw time-based curve
    base_curve = std::get<std::vector<std::tuple<double, double>>>(curve);
  }
  else
  {
    throw std::runtime_error("Mismatch between curve and valuation date types in InterestRateCurve.");
  }

  return interpolate_rate_curve(base_curve, curve_to_interpolate);
}
