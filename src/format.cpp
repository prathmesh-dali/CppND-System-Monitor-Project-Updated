#include "format.h"

#include <chrono>
#include <iomanip>
#include <string>

using std::string;

// INPUT: Long int measuring seconds
// OUTPUT: HH:MM:SS
string Format::ElapsedTime(long seconds) {
  int hours, minutes;
  // hours = seconds / (60 * 60);
  // minutes = (seconds / 60) % 60;
  // seconds = seconds % 60;
  std::chrono::seconds secs{seconds};
  auto hrs = std::chrono::duration_cast<std::chrono::hours>(secs);
  secs -= std::chrono::duration_cast<std::chrono::seconds>(hrs);
  auto mins = std::chrono::duration_cast<std::chrono::minutes>(secs);
  secs -= std::chrono::duration_cast<std::chrono::seconds>(mins);
  seconds = (std::chrono::duration_cast<std::chrono::seconds>(secs)).count();
  hours = hrs.count();
  minutes = mins.count();

  std::stringstream result;

  result << std::setfill('0') << std::setw(2) << hours << ":";
  result << std::setfill('0') << std::setw(2) << minutes << ":";
  result << std::setfill('0') << std::setw(2) << seconds;
  return result.str();
}