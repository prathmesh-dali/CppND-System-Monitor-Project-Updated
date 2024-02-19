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
      command_(LinuxParser::Command(pid)),
      ram_(LinuxParser::Ram(pid)),
      uptime_(LinuxParser::UpTime(pid)),
      user_(LinuxParser::User(pid)) {
  long totalTime = LinuxParser::ActiveJiffies(pid);
  long seconds = LinuxParser::UpTime() - uptime_;
  cpuUtilization_ = float(totalTime) / float(seconds);
}

int Process::Pid() const { return pid_; }

float Process::CpuUtilization() const { return cpuUtilization_; }

string Process::Command() { return command_; }

string Process::Ram() { return ram_; }

string Process::User() { return user_; }

long int Process::UpTime() { return uptime_; }

bool Process::operator<(Process const& a) const {
  return CpuUtilization() < a.cpuUtilization_;
}