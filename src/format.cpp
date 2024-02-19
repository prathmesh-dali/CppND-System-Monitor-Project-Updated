#include "format.h"

#include <string>
#include <iomanip>

using std::string;

// INPUT: Long int measuring seconds
// OUTPUT: HH:MM:SS
string Format::ElapsedTime(long seconds) {
  int hours, minutes;
  hours = seconds / (60 * 60);
  minutes = (seconds / 60) % 60;
  seconds = seconds % 60;

  std::stringstream result;

  result << std::setfill('0') << std::setw(2) << hours << ":";
  result << std::setfill('0') << std::setw(2) << minutes << ":";
  result << std::setfill('0') << std::setw(2) << seconds;
  return result.str();
}