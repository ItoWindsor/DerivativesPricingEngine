#include <chrono>
#include "instruments/Bond.hpp"
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

const std::vector<std::chrono::sys_days>& Bond::get_vec_date_cashflows() const{
  return this->vec_date_cashflow;
}

std::ostream& operator<<(std::ostream& output_stream, const Bond& bond) {

  output_stream << "Bond Termsheet" << std::endl;
  output_stream << "Start Date: " << std::chrono::year_month_day{bond.get_start_date()} << std::endl;
  output_stream << "Valuation Date: " << std::chrono::year_month_day{bond.get_valuation_date()} << std::endl;
  output_stream << "Face Value: " << bond.get_nominal() << std::endl;
  output_stream << "Spread: " << bond.get_spread() << std::endl;
  output_stream << "Compounding Method: " << bond.get_compounding_method() << std::endl;
  output_stream << "Compounding Frequency: " << bond.get_compounding_frequency() << std::endl;
  output_stream << "Day Count Convention: " << bond.get_day_convention() << std::endl;
  output_stream << "Payment schedule: " << std::endl;
  for(auto date : bond.get_vec_date_cashflows()){
    output_stream << date << std::endl;
  }

  return output_stream;
}
