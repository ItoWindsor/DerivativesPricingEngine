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
      DayCountConvention day_count_convention,
      CompoundingConvention CompoundingConvention,
      double Nominal,
      double coupon_rate,
      double spread,
      std::vector<std::chrono::sys_days> vec_date_cashflow
    );    
    double get_nominal() const;
    double get_spread() const;
    double get_coupon_rate() const;
    CompoundingConvention get_compounding_convention() const;
    const std::vector<std::chrono::sys_days>& get_vec_date_cashflows() const;
  private:
    CompoundingConvention compounding_convention;
    double nominal;
    double coupon_rate;
    double spread;
    std::vector<std::chrono::sys_days> vec_date_cashflow; 
};

#endif
