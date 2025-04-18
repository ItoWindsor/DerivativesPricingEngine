#include <chrono>
#include "instruments/Bond.hpp"

Bond::Bond(
      std::chrono::sys_days start_date,
      std::chrono::sys_days valuation_date,
      std::chrono::sys_days maturity_date,
      double Nominal,
      double spread,
      std::vector<std::chrono::sys_days> vec_date_cashflow)
  : Instrument(start_date,valuation_date,maturity_date), nominal(Nominal), spread(spread), vec_date_cashflow(vec_date_cashflow) {}

double Bond::get_nominal() const{
  return this->nominal;
}


double Bond::get_spread() const{
  return this->spread;
}

std::vector<std::chrono::sys_days>& Bond::get_vec_date_cashflows(){
  return this->vec_date_cashflow;
}
