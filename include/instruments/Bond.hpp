#ifndef BOND_HPP
#define BOND_HPP

#include "core/Instrument.hpp"

class Bond : public Instrument{
  public:
    
    double get_nominal() const;
    double get_spread() const;
    std::vector<std::chrono::sys_days> get_vec_date_cashflows();
  private:
    double nominal;
    double spread;
    std::vector<std::chrono::sys_days> vec_date_cashflow; 
};

#endif
