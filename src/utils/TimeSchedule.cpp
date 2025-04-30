#include<vector>
#include <chrono>
#include "enums/CompoundingConvention.hpp"
#include "enums/DayCountConvention.hpp"
#include "utils/TimeSchedule.hpp"

std::chrono::year_month_day add_period(
  const std::chrono::year_month_day& date,
  CompoundingFrequency freq) {
  
  switch (freq) {
    case CompoundingFrequency::anually:
      return date + std::chrono::years{1};
    case CompoundingFrequency::semianually:
      return date + std::chrono::months{6};
    case CompoundingFrequency::quarterly:
      return date + std::chrono::months{3};
    case CompoundingFrequency::monthly:
      return date + std::chrono::months{1};
    default:
      throw std::invalid_argument("Unsupported frequency");
  }

}


std::chrono::year_month_day subtract_period(
  const std::chrono::year_month_day& date,
  CompoundingFrequency freq){ 
  
  switch (freq) {
    case CompoundingFrequency::anually:
      return date - std::chrono::years{1};
    case CompoundingFrequency::semianually:
      return date - std::chrono::months{6};
    case CompoundingFrequency::quarterly:
      return date - std::chrono::months{3};
    case CompoundingFrequency::monthly:
      return date - std::chrono::months{1};
    default:
      throw std::invalid_argument("Unsupported frequency");
  }

}


std::vector<std::chrono::sys_days> generate_time_schedule(
  std::chrono::sys_days start_date,
  std::chrono::sys_days end_date,
  CompoundingFrequency compounding_frequency,
  ShortSchedule short_schedule){
  
  std::vector<std::chrono::sys_days> vec_date;
  std::chrono::year_month_day start_c{start_date};
  std::chrono::year_month_day end_c{end_date};

  switch (short_schedule) {

    case ShortSchedule::ShortStart : {
      std::vector<std::chrono::sys_days> reversed;
      std::chrono::year_month_day current = end_c;
      while(current >= start_c){
        reversed.push_back(std::chrono::sys_days{current});
        current = subtract_period(current,compounding_frequency);
      }

      if (reversed.back() != start_date){
        reversed.push_back(start_date);
      }
      
      std::reverse(reversed.begin(), reversed.end());
      vec_date = std::move(reversed);
      break;
    }

    case ShortSchedule::ShortEnd : {
      std::chrono::year_month_day current = start_c;
      while(current <= end_c){
        vec_date.push_back(std::chrono::sys_days{current});
        current = add_period(current,compounding_frequency);
      }

      if (vec_date.back() != end_date){
        vec_date.push_back(end_date);
      }
      break;
    }
  }

  return vec_date; 
}

std::chrono::sys_days make_date(int year, unsigned int month, unsigned int day) {
    return std::chrono::sys_days{
        std::chrono::year{year} / std::chrono::month{month} / std::chrono::day{day}
    };
}
