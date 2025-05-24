#ifndef SWAP_HPP
#define SWAP_HPP

#include <chrono>
#include "core/Instrument.hpp"
#include "enums/CompoundingConvention.hpp"
#include "enums/DayCountConvention.hpp"

class Swap : public Instrument {
  public:
    Swap(
      std::chrono::sys_days start_date,
      std::chrono::sys_days valuation_date,
      std::chrono::sys_days maturity_date,
      double Nominal,
      double coupon_rate,
      double spread,
      std::vector<std::chrono::sys_days> vec_date_cashflow_fixed_leg,
      DayCountConvention day_count_convention = DayCountConvention::Actual365,
      CompoundingMethod compounding_method = CompoundingMethod::actuarial,
      CompoundingFrequency compounding_frequency = CompoundingFrequency::anually
    );
  private:
    CompoundingMethod compounding_method_fixed_leg;
    CompoundingFrequency compounding_frequency_fixed_leg;
    
    CompoundingMethod compounding_method_floating_leg;
    CompoundingFrequency compounding_frequency_floating_leg;
  
    double nominal;
    double coupon_rate;
    double spread;

    std::variant<std::vector<std::chrono::sys_days>,std::vector<double>> vec_date_cashflow_fixed_leg;
    std::variant<std::vector<std::chrono::sys_days>,std::vector<double>> vec_date_cashflow_floating_leg; 
};


#endif
