#include <chrono>
#include "utils/DayCount.hpp"

double year_fraction(
  std::chrono::sys_days start,
  std::chrono::sys_days end,
  DayCountConvention convention) {

  std::chrono::days days_duration = std::chrono::duration_cast<std::chrono::days>(end - start);
    double days = days_duration.count();

    switch (convention) {
        case DayCountConvention::Actual360:
            return static_cast<double>(days) / 360.0;
        case DayCountConvention::Actual365:
            return static_cast<double>(days) / 365.0;
    }
}
