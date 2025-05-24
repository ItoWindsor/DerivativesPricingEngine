#include "instruments/equity/CallOption.hpp"
#include "utils/UsualPayoffs.hpp"

CallOption::CallOption(
      std::chrono::sys_days start_date, 
      std::chrono::sys_days valuation_date,
      std::chrono::sys_days maturity_date,
      double strike,
      ExerciseKind exercise_kind,
      bool is_path_dependent)
  : Option(start_date,valuation_date, maturity_date, strike, call_payoff, exercise_kind, is_path_dependent) {}


CallOption::CallOption(
  double start_time, 
  double valuation_time,
  double maturity_time,
  double strike,
  ExerciseKind exercise_kind,
  bool is_path_dependent)
  : Option(start_time,valuation_time, maturity_time, strike, call_payoff, exercise_kind, is_path_dependent) {}
