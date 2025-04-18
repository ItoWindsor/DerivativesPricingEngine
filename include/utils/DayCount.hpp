#ifndef DAYCOUNT_HPP
#define DAYCOUNT_HPP

#include <chrono>
#include "enums/DayCountConvention.hpp"

double year_fraction(
    std::chrono::sys_days start,
    std::chrono::sys_days end,
    DayCountConvention convention);

#endif
