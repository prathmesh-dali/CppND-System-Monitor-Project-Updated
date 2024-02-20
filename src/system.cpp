#include "system.h"

#include <unistd.h>

#include <cstddef>
#include <set>
#include <string>
#include <vector>

#include "linux_parser.h"
#include "process.h"
#include "processor.h"

using std::set;
using std::size_t;
using std::string;
using std::vector;

Processor& System::Cpu() { return cpu_; }

vector<Process>& System::Processes() {
  vector<int> pids = LinuxParser::Pids();

  set<int> existing_processe;
  for (auto it = processes_.begin(); it != processes_.end();) {
    if (std::find(pids.begin(), pids.end(), it->Pid()) == pids.end()) {
      processes_.erase(it);
    } else {
      ++it;
    }
  }
  for (auto& process : processes_) {
    existing_processe.emplace(process.Pid());
  }
  for (int pid : pids) {
    if (existing_processe.find(pid) == existing_processe.end()) {
      processes_.emplace_back(Process(pid));
    }
  }
  for (Process& process : processes_) {
    process.CpuUtilization(LinuxParser::ActiveJiffies(process.Pid()),
                           LinuxParser::Jiffies());
  }
  std::sort(processes_.rbegin(), processes_.rend());
  return processes_;
}

std::string System::Kernel() { return LinuxParser::Kernel(); }

float System::MemoryUtilization() { return LinuxParser::MemoryUtilization(); }

std::string System::OperatingSystem() { return LinuxParser::OperatingSystem(); }

int System::RunningProcesses() { return LinuxParser::RunningProcesses(); }

int System::TotalProcesses() { return LinuxParser::TotalProcesses(); }

long int System::UpTime() { return LinuxParser::UpTime(); }