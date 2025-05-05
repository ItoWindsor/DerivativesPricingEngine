#include <chrono>
#include "instruments/interest_rates/Bond.hpp"
#include "enums/CompoundingConvention.hpp"
#include "enums/DayCountConvention.hpp"
#include "utils/TimeSchedule.hpp"
#include "utils/EnumToString.hpp"

Bond::Bond(
  std::chrono::sys_days start_date,
  std::chrono::sys_days valuation_date,
  std::chrono::sys_days maturity_date,
  double Nominal,
  double coupon_rate,
  double spread,
  std::vector<std::chrono::sys_days> vec_date_cashflow,
  DayCountConvention day_count_convention,
  CompoundingMethod compounding_method,
  CompoundingFrequency compounding_frequency)
  : Instrument(start_date,valuation_date,maturity_date,day_count_convention), compounding_method(compounding_method), compounding_frequency(compounding_frequency) , nominal(Nominal), coupon_rate(coupon_rate), spread(spread), vec_date_cashflow(vec_date_cashflow) {}


Bond::Bond(
  std::chrono::sys_days start_date,
  std::chrono::sys_days valuation_date,
  std::chrono::sys_days maturity_date,
  double Nominal,
  double coupon_rate,
  double spread,
  DayCountConvention day_count_convention,
  CompoundingMethod compounding_method,
  CompoundingFrequency compounding_frequency,
  ShortSchedule short_schedule)
  : Instrument(start_date,valuation_date,maturity_date,day_count_convention), compounding_method(compounding_method), compounding_frequency(compounding_frequency), nominal(Nominal), coupon_rate(coupon_rate), spread(spread) {
  vec_date_cashflow = generate_time_schedule(start_date, maturity_date,compounding_frequency, short_schedule); 
};


Bond::Bond(
  double start_time,
  double valuation_time,
  double maturity_time,
  double Nominal,
  double coupon_rate,
  double spread,
  CompoundingMethod compounding_method,
  CompoundingFrequency compounding_frequency,
  ShortSchedule short_schedule)
  : Instrument(start_time,valuation_time,maturity_time), compounding_method(compounding_method), compounding_frequency(compounding_frequency), nominal(Nominal), coupon_rate(coupon_rate), spread(spread) {
  vec_date_cashflow = generate_time_schedule(start_time, maturity_time,compounding_frequency, short_schedule); 
};
double Bond::get_nominal() const{
  return this->nominal;
}

double Bond::get_spread() const{
  return this->spread;
}

double Bond::get_coupon_rate() const{
  return this->coupon_rate;
}

CompoundingMethod Bond::get_compounding_method() const{
  return this->compounding_method;
}

CompoundingFrequency Bond::get_compounding_frequency() const{
  return this->compounding_frequency;
}

const std::variant<std::vector<std::chrono::sys_days>, std::vector<double>>& Bond::get_vec_date_cashflows() const {
  return this->vec_date_cashflow;
}

const std::variant<std::vector<std::chrono::sys_days>, std::vector<double>> Bond::get_vec_futur_cashflows() const {
  auto valuation_time_or_date = this->get_valuation_date();
  auto vec_all_cashflows = this->get_vec_date_cashflows();

  if (std::holds_alternative<std::chrono::sys_days>(valuation_time_or_date) &&
      std::holds_alternative<std::vector<std::chrono::sys_days>>(vec_all_cashflows)) 
  {
      const auto& val_date = std::get<std::chrono::sys_days>(valuation_time_or_date);
      const auto& all_cashflows = std::get<std::vector<std::chrono::sys_days>>(vec_all_cashflows);

      std::vector<std::chrono::sys_days> future_dates;
      for (const auto& t : all_cashflows) {
          if (t >= val_date) {
              future_dates.push_back(t);
          }
      }
      return future_dates;
  }
  else if (std::holds_alternative<double>(valuation_time_or_date) &&
           std::holds_alternative<std::vector<double>>(vec_all_cashflows)) 
  {
      const auto& val_date = std::get<double>(valuation_time_or_date);
      const auto& all_cashflows = std::get<std::vector<double>>(vec_all_cashflows);

      std::vector<double> future_values;
      for (const auto& t : all_cashflows) {
          if (t >= val_date) {
              future_values.push_back(t);
          }
      }
      return future_values;
  }
  else {
      throw std::runtime_error("Mismatched types in valuation_date and cashflows");
  }
}

std::ostream& operator<<(std::ostream& output_stream, const Bond& bond) {
    
  auto print_value = [&output_stream](const auto& value) {
    if constexpr (std::is_same_v<std::decay_t<decltype(value)>, std::chrono::sys_days>) {
        output_stream << std::chrono::year_month_day{value};
    } 
    else {
        output_stream << value;
    }
  };

  output_stream << "Bond Termsheet" << std::endl;

  output_stream << "Start Date: ";
  std::visit([&](const auto& date) { print_value(date); }, bond.get_start_date());
  output_stream << std::endl;

  output_stream << "Valuation Date: ";
  std::visit([&](const auto& date) { print_value(date); }, bond.get_valuation_date());
  output_stream << std::endl;

  output_stream << "Face Value: " << bond.get_nominal() << std::endl;
  output_stream << "Spread: " << bond.get_spread() << std::endl;
  output_stream << "Compounding Method: " << bond.get_compounding_method() << std::endl;
  output_stream << "Compounding Frequency: " << bond.get_compounding_frequency() << std::endl;
  output_stream << "Day Count Convention: " << bond.get_day_convention() << std::endl;

  output_stream << "Payment Schedule: " << std::endl;
  std::visit([&](const auto& dates) {
      for (const auto& date : dates) {
          print_value(date);
          output_stream << std::endl;
      }
  }, bond.get_vec_date_cashflows());

  return output_stream;
}
