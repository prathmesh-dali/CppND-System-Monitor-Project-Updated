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
    : pid_(pid) {
}

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

string Process::Command() { return LinuxParser::Command(Pid()); }

string Process::Ram() { return LinuxParser::Ram(Pid()); }

string Process::User() { return LinuxParser::User(Pid()); }

long int Process::UpTime() { return LinuxParser::UpTime(Pid()); }

bool Process::operator<(Process const& a) const {
  return CpuUtilization() < a.cpuUtilization_;
}