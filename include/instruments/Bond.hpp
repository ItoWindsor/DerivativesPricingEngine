#ifndef BOND_HPP
#define BOND_HPP

#include <chrono>
#include "core/Instrument.hpp"
#include "enums/CompoundingConvention.hpp"
#include "enums/DayCountConvention.hpp"

class Bond : public Instrument{
  public:
    Bond(
      std::chrono::sys_days start_date,
      std::chrono::sys_days valuation_date,
      std::chrono::sys_days maturity_date,
      double Nominal,
      double coupon_rate,
      double spread,
      std::vector<std::chrono::sys_days> vec_date_cashflow,
      DayCountConvention day_count_convention = DayCountConvention::Actual365,
      CompoundingMethod compounding_method = CompoundingMethod::actuarial,
      CompoundingFrequency compounding_frequency = CompoundingFrequency::anually
    );   
    Bond(
      std::chrono::sys_days start_date,
      std::chrono::sys_days valuation_date,
      std::chrono::sys_days maturity_date,
      double Nominal,
      double coupon_rate,
      double spread,
      DayCountConvention day_count_convention = DayCountConvention::Actual365,
      CompoundingMethod compounding_method = CompoundingMethod::actuarial,
      CompoundingFrequency compounding_frequency = CompoundingFrequency::anually,
      ShortSchedule short_schedule = ShortSchedule::ShortEnd
    );
    double get_nominal() const;
    double get_spread() const;
    double get_coupon_rate() const;
    CompoundingMethod get_compounding_method() const;
    CompoundingFrequency get_compounding_frequency() const;
    const std::vector<std::chrono::sys_days>& get_vec_date_cashflows() const;
  private:
    CompoundingMethod compounding_method;
    CompoundingFrequency compounding_frequency;
    double nominal;
    double coupon_rate;
    double spread;
    std::vector<std::chrono::sys_days> vec_date_cashflow; 
};


std::ostream& operator<<(std::ostream& output_stream,const Bond& bond);

#endif
