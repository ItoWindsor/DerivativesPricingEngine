#include <chrono>
#include "instruments/Bond.hpp"
#include "enums/CompoundingConvention.hpp"
#include "enums/DayCountConvention.hpp"

Bond::Bond(
      std::chrono::sys_days start_date,
      std::chrono::sys_days valuation_date,
      std::chrono::sys_days maturity_date,
      DayCountConvention day_count_convention,
      CompoundingConvention compounding_convention,
      double Nominal,
      double coupon_rate,
      double spread,
      std::vector<std::chrono::sys_days> vec_date_cashflow)
  : Instrument(start_date,valuation_date,maturity_date,day_count_convention), compounding_convention(compounding_convention), nominal(Nominal), coupon_rate(coupon_rate), spread(spread), vec_date_cashflow(vec_date_cashflow) {}

double Bond::get_nominal() const{
  return this->nominal;
}

double Bond::get_spread() const{
  return this->spread;
}

double Bond::get_coupon_rate() const{
  return this->coupon_rate;
}

CompoundingConvention Bond::get_compounding_convention() const{
  return this->compounding_convention;
}

const std::vector<std::chrono::sys_days>& Bond::get_vec_date_cashflows() const{
  return this->vec_date_cashflow;
}
