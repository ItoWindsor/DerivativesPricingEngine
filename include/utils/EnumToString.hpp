#ifndef ENUM_TO_STRING_HPP
#define ENUM_TO_STRING_HPP

#include <ostream>
#include "enums/CompoundingConvention.hpp"
#include "enums/DayCountConvention.hpp"

std::ostream& operator<<(std::ostream& os, CompoundingMethod method);
std::ostream& operator<<(std::ostream& os, CompoundingFrequency freq);
std::ostream& operator<<(std::ostream& os, DayCountConvention dcc);
std::ostream& operator<<(std::ostream& os, ShortSchedule sched);

#endif 
