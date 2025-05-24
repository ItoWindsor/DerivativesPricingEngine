#ifndef CALLOPTION_HPP
#define CALLOPTION_HPP

#include <iostream>
#include "instruments/equity/Option.hpp"

class CallOption : public Option{
public : 
  CallOption(
    std::chrono::sys_days start_date, 
    std::chrono::sys_days valuation_date,
    std::chrono::sys_days maturity_date,
    double strike,
    ExerciseKind exercise_kind,
    bool is_path_dependent);
  
  CallOption(
    double start_time, 
    double valuation_time,
    double maturity_time,
    double strike,
    ExerciseKind exercise_kind,
    bool is_path_dependent);
};


std::ostream& operator<<(std::ostream& output_stream,const CallOption& call);

#endif 
