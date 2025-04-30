#ifndef OPTION_HPP
#define OPTION_HPP

#include <functional>
#include "core/Instrument.hpp"
#include "enums/ExerciseConvention.hpp"

class Option : public Instrument{
  public:
    Option(
      std::chrono::sys_days start_date, 
      std::chrono::sys_days valuation_date,
      std::chrono::sys_days maturity_date,
      double strike,
      std::function<double(double,double)>  payoff,
      ExerciseKind exercise_kind,
      bool is_path_dependent);
    double get_strike() const;
    std::function<double(double,double)> get_payoff() const;
    ExerciseKind get_exercise_kind() const;
  private:
    double strike;
    std::function<double(double,double)> payoff;
    ExerciseKind exerciseKind;
    bool isPathDependent;
};



#endif
