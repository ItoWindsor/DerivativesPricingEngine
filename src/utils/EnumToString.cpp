#include <ostream>
#include "utils/EnumToString.hpp"
#include "enums/CompoundingConvention.hpp"
#include "enums/DayCountConvention.hpp"

std::ostream& operator<<(std::ostream& os, CompoundingMethod method) {
  switch (method) {
    case CompoundingMethod::actuarial: os << "Actuarial"; break;
    case CompoundingMethod::continuous: os << "Continuous"; break;
    default: os << "Unknown"; break;
  }
  return os;
}

std::ostream& operator<<(std::ostream& os, CompoundingFrequency freq) {
  switch (freq) {
    case CompoundingFrequency::anually: os << "Annual"; break;
    case CompoundingFrequency::semianually: os << "Semi-Annual"; break;
    case CompoundingFrequency::quarterly: os << "Quarterly"; break;
    case CompoundingFrequency::monthly: os << "Monthly"; break;
    default: os << "Unknown"; break;
  }
  return os;
}

std::ostream& operator<<(std::ostream& os, DayCountConvention dcc) {
  switch (dcc) {
    case DayCountConvention::Actual360: os << "Actual/360"; break;
    case DayCountConvention::Actual365: os << "Actual/365"; break;
    default: os << "Unknown"; break;
  }
  return os;
}

std::ostream& operator<<(std::ostream& os, ShortSchedule sched) {
  switch (sched) {
    case ShortSchedule::ShortStart: os << "Short Start"; break;
    case ShortSchedule::ShortEnd: os << "Short End"; break;
    default: os << "Unknown"; break;
  }
  return os;
}
