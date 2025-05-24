#include "instruments/equity/Option.hpp"
#include "core/Instrument.hpp"
#include "enums/ExerciseConvention.hpp"
#include <functional>

Option::Option(
  std::chrono::sys_days start_date, 
  std::chrono::sys_days valuation_date,
  std::chrono::sys_days maturity_date,
  double strike,
  std::function<double(double,double)>  payoff,
  ExerciseKind exercise_kind,
  bool is_path_dependent)
  : Instrument(start_date, valuation_date, maturity_date), strike(strike), payoff(payoff), exerciseKind(exercise_kind), isPathDependent(is_path_dependent) {} 


Option::Option(
  double start_time, 
  double valuation_time,
  double maturity_time,
  double strike,
  std::function<double(double,double)>  payoff,
  ExerciseKind exercise_kind,
  bool is_path_dependent)
  : Instrument(start_time, valuation_time, maturity_time), strike(strike), payoff(payoff), exerciseKind(exercise_kind), isPathDependent(is_path_dependent) {} 

double Option::get_strike() const{
  return this->strike;
}

std::function<double(double,double)> Option::get_payoff() const{
  return this->payoff;
}

ExerciseKind Option::get_exercise_kind() const{
  return this->exerciseKind;
}

bool Option::get_path_dependent() const{
  return this->isPathDependent;
}
