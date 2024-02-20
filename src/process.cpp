#include "process.h"

#include <unistd.h>

#include <cctype>
#include <sstream>
#include <string>
#include <vector>

#include "linux_parser.h"

using std::string;
using std::to_string;
using std::vector;

Process::Process(int pid)
    : pid_(pid),
      user_(LinuxParser::User(pid)),
      command_(LinuxParser::Command(pid)),
      uptime_(LinuxParser::UpTime(pid)) {}

int Process::Pid() const { return pid_; }

float Process::CpuUtilization() const { return cpuUtilization_; }
void Process::CpuUtilization(long processJiffes, long totalJiffes) {
  long deltaProcessJiffes, deltaTotalJiffes;
  deltaProcessJiffes = processJiffes - previousProcessJiffes_;
  deltaTotalJiffes = totalJiffes - previousTotalJiffes_;
  previousProcessJiffes_ = processJiffes;
  previousTotalJiffes_ = totalJiffes;
  cpuUtilization_ = float(deltaProcessJiffes) / float(deltaTotalJiffes);
}

string Process::Command() { return command_; }

string Process::Ram() { return LinuxParser::Ram(Pid()); }

string Process::User() { return user_; }

long int Process::UpTime() {
  return uptime_ > 0 ? LinuxParser::UpTime() - uptime_ : uptime_;
}

bool Process::operator<(Process const& a) const {
  return CpuUtilization() < a.cpuUtilization_;
}