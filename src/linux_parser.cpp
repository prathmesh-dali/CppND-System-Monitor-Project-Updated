#include "linux_parser.h"

#include <dirent.h>
#include <unistd.h>

#include <string>
#include <vector>

using std::stof;
using std::string;
using std::to_string;
using std::vector;

// DONE: An example of how to read data from the filesystem
string LinuxParser::OperatingSystem() {
  string line;
  string key;
  string value;
  std::ifstream filestream(kOSPath);
  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      std::replace(line.begin(), line.end(), ' ', '_');
      std::replace(line.begin(), line.end(), '=', ' ');
      std::replace(line.begin(), line.end(), '"', ' ');
      std::istringstream linestream(line);
      while (linestream >> key >> value) {
        if (key == "PRETTY_NAME") {
          std::replace(value.begin(), value.end(), '_', ' ');
          return value;
        }
      }
    }
  }
  return value;
}

// DONE: An example of how to read data from the filesystem
string LinuxParser::Kernel() {
  string os, version, kernel;
  string line;
  std::ifstream stream(kProcDirectory + kVersionFilename);
  if (stream.is_open()) {
    std::getline(stream, line);
    std::istringstream linestream(line);
    linestream >> os >> version >> kernel;
  }
  return kernel;
}

// BONUS: Update this to use std::filesystem
vector<int> LinuxParser::Pids() {
  vector<int> pids;
  DIR* directory = opendir(kProcDirectory.c_str());
  struct dirent* file;
  while ((file = readdir(directory)) != nullptr) {
    // Is this a directory?
    if (file->d_type == DT_DIR) {
      // Is every character of the name a digit?
      string filename(file->d_name);
      if (std::all_of(filename.begin(), filename.end(), isdigit)) {
        int pid = stoi(filename);
        pids.push_back(pid);
      }
    }
  }
  closedir(directory);
  return pids;
}

float LinuxParser::MemoryUtilization() {
  string token, mem, line;
  float totalmemory, freememory;
  std::ifstream stream(kProcDirectory + kMeminfoFilename);
  if (stream.is_open()) {
    while (std::getline(stream, line)) {
      std::istringstream linestream(line);
      linestream >> token >> mem;
      if (token == kTotalMemory) {
        totalmemory = stof(mem);
      } else if (token == kFreeMemory) {
        freememory = stof(mem);
      }
    }
  }
  return (totalmemory - freememory) / totalmemory;
}

long LinuxParser::UpTime() {
  string uptimestr, line;
  std::ifstream stream(kProcDirectory + kUptimeFilename);
  if (stream.is_open()) {
    std::getline(stream, line);
    std::istringstream linestream(line);
    linestream >> uptimestr;
  }
  return std::stol(uptimestr);
}

long LinuxParser::Jiffies() { return ActiveJiffies() + IdleJiffies(); }

long LinuxParser::ActiveJiffies(int pid) {
  std::string line, value;
  vector<std::string> values;
  long totaltime{0};
  std::ifstream stream(kProcDirectory + std::to_string(pid) + kStatFilename);
  if (stream.is_open()) {
    getline(stream, line);
    std::istringstream linestream(line);
    while (linestream >> value) {
      values.push_back(value);
    }
    const int utime{13}, stime{14}, cutime{15}, cstime{16};
    if (values.size() >= cstime) {
      totaltime = std::stol(values[utime]) + std::stol(values[stime]) +
                  std::stol(values[cutime]) + std::stol(values[cstime]);
    }
  }
  return totaltime;
}

long LinuxParser::ActiveJiffies() {
  vector<string> jiffies = CpuUtilization();
  return std::stol(jiffies[CPUStates::kIRQ_]) +
         std::stol(jiffies[CPUStates::kGuest_]) +
         std::stol(jiffies[CPUStates::kGuestNice_]) +
         std::stol(jiffies[CPUStates::kNice_]) +
         std::stol(jiffies[CPUStates::kSoftIRQ_]) +
         std::stol(jiffies[CPUStates::kSteal_]) +
         std::stol(jiffies[CPUStates::kSystem_]) +
         std::stol(jiffies[CPUStates::kUser_]);
}

long LinuxParser::IdleJiffies() {
  vector<string> jiffies = CpuUtilization();
  return std::stol(jiffies[CPUStates::kIdle_]) +
         std::stol(jiffies[CPUStates::kIOwait_]);
}

vector<string> LinuxParser::CpuUtilization() {
  vector<string> jiffies;
  string line, value;
  std::ifstream stream(kProcDirectory + kStatFilename);
  if (stream.is_open()) {
    std::getline(stream, line);
    std::istringstream linestream(line);
    linestream >> value;
    while (linestream >> value) {
      jiffies.emplace_back(value);
    }
  }
  return jiffies;
}

int LinuxParser::TotalProcesses() {
  int totalProcesses{0};
  string line, key, value;
  std::ifstream stream(kProcDirectory + kStatFilename);
  if (stream.is_open()) {
    while (std::getline(stream, line)) {
      std::istringstream linestream(line);
      linestream >> key >> value;
      if (key == kTotalProcesses) {
        totalProcesses = std::stoi(value);
        break;
      }
    }
  }
  return totalProcesses;
}

int LinuxParser::RunningProcesses() {
  int runningProcesses{0};
  string line, key, value;
  std::ifstream stream(kProcDirectory + kStatFilename);
  if (stream.is_open()) {
    while (std::getline(stream, line)) {
      std::istringstream linestream(line);
      linestream >> key >> value;
      if (key == kRunningProcesses) {
        runningProcesses = std::stoi(value);
        break;
      }
    }
  }
  return runningProcesses;
}

string LinuxParser::Command(int pid) {
  string cmd;
  std::ifstream stream(kProcDirectory + std::to_string(pid) + kCmdlineFilename);
  if (stream.is_open()) {
    std::getline(stream, cmd);
  }
  return cmd;
}

string LinuxParser::Ram(int pid) {
  string line, key, value, memoryUtilization{"0"};
  std::ifstream stream(kProcDirectory + std::to_string(pid) + kStatusFilename);
  if (stream.is_open()) {
    while (std::getline(stream, line)) {
      std::istringstream linestream(line);
      linestream >> key;
      if (key == kProcessMemory) {
        linestream >> value;
        break;
      }
    }
  }
  try {
    memoryUtilization = std::to_string(std::stoi(value) / 1024);
  } catch (...) {
    memoryUtilization = "0";
  }
  return memoryUtilization;
}

string LinuxParser::Uid(int pid) {
  string line, key, value, uid;
  std::ifstream stream(kProcDirectory + std::to_string(pid) + kStatusFilename);
  if (stream.is_open()) {
    while (std::getline(stream, line)) {
      std::istringstream linestream(line);
      linestream >> key;
      if (key == kUid) {
        linestream >> uid;
        break;
      }
    }
  }
  return uid;
}

string LinuxParser::User(int pid) {
  string uid{Uid(pid)}, line, user, uid_file, dummy;
  std::ifstream stream(kPasswordPath);
  if (stream.is_open()) {
    while (getline(stream, line)) {
      std::replace(line.begin(), line.end(), ':', ' ');
      std::istringstream linestream(line);
      linestream >> user >> dummy >> uid_file;
      if (uid_file == uid) {
        break;
      }
    }
  }
  return user;
}

long LinuxParser::UpTime(int pid) {
  std::string line, value;
  long uptime{0};
  vector<string> values;
  std::ifstream stream(kProcDirectory + std::to_string(pid) + kStatFilename);
  if (stream.is_open()) {
    std::getline(stream, line);
    std::istringstream linestream(line);
    while (linestream >> value) {
      values.push_back(value);
    }
  }
  const int upTimePosition = 21;
  if (values.size() > upTimePosition) {
    try {
      uptime = stol(values[upTimePosition]) / sysconf(_SC_CLK_TCK);
    } catch (...) {
      uptime = 0;
    }
  }
  return uptime;
}
