
#include <dirent.h>
#include <unistd.h>
#include "linux_parser.h"

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
  string os, kernel;
  string line;
  std::ifstream stream(kProcDirectory + kVersionFilename);
  if (stream.is_open()) {
    std::getline(stream, line);
    std::istringstream linestream(line);
    linestream >> os >> kernel;
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

// TODO: Read and return the system memory utilization
float LinuxParser::MemoryUtilization() {
  string line, key, unit;
  double value{0.0};
  vector<double> mem_data{0.0};
  std::ifstream stream(kProcDirectory + kMeminfoFilename);
  for (int i{0}; i < 5; ++i) {
    std::getline(stream, line);
    std::istringstream linestream(line);
    linestream >> key >> value >> unit;
    mem_data.push_back(value);
  }
  float mem_utilization =
      (mem_data.at(MemoryType::kMemTotal) - mem_data.at(MemoryType::kMemFree) -
      mem_data.at(MemoryType::kBuffers) - mem_data.at(MemoryType::kCached)) / mem_data.at(MemoryType::kMemTotal);
  
  return mem_utilization;
}

// TODO: Read and return the system uptime
long LinuxParser::UpTime() {
  long value{0};
  string line;
  std::ifstream stream(kProcDirectory + kUptimeFilename);
  if (stream.is_open()) {
    std::getline(stream, line);
    std::istringstream linestream(line);
    linestream >> value;
    return value;
  }
  return value;
}

// TODO: Read and return the number of jiffies for the system
long LinuxParser::Jiffies() {
  string line, data;
  double value{0.0};
  long jiffies{0};
  std::ifstream stream(kProcDirectory + kStatFilename);
  if (stream.is_open()) {
    std::getline(stream, line);
    std::istringstream linestream(line);
    linestream >> data;
    while (linestream >> value) {
      jiffies += value;
    }
  }
  return jiffies;
}

// TODO: Read and return the number of active jiffies for a PID
// REMOVE: [[maybe_unused]] once you define the function
long LinuxParser::ActiveJiffies(int pid) {
  long int value, active_pid_jiffies{0};
  string line, key;
  std::ifstream stream(kProcDirectory + to_string(pid) + kStatFilename);
  if (stream.is_open()) {
    std::getline(stream, line); 
    std::istringstream linestream(line);
    for (int i{0}; i < 13; ++i) linestream >> key;
    for (int i{0}; i < 4; ++i) {
      linestream >> value;
      active_pid_jiffies += value;
    }
    return active_pid_jiffies;
  }
  return active_pid_jiffies;
}

// TODO: Read and return the number of active jiffies for the system
long LinuxParser::ActiveJiffies() {
  string line, key;
  double value{0.0};
  vector<int> data;
  std::ifstream stream(kProcDirectory + kStatFilename);
  if (stream.is_open()) {
    std::getline(stream, line);
    std::istringstream linestream(line);
    linestream >> key;
    while (linestream >> value) {
      data.push_back(value);
    }
  }
  long active_jiffies =
      data.at(CPUStates::kUser_) + data.at(CPUStates::kNice_) +
      data.at(CPUStates::kSystem_) + data.at(CPUStates::kIRQ_) +
      data.at(CPUStates::kSoftIRQ_) + data.at(CPUStates::kSteal_);
  return active_jiffies;
}

// TODO: Read and return the number of idle jiffies for the system
long LinuxParser::IdleJiffies() {
  string line, key;
  double value{0.0};
  vector<int> data;
  std::ifstream stream(kProcDirectory + kStatFilename);
  if (stream.is_open()) {
    std::getline(stream, line);
    std::istringstream linestream(line);
    linestream >> key;
    while (linestream >> value) {
      data.push_back(value);
    }
  }
  long idle_jiffies = data.at(CPUStates::kIdle_) + data.at(CPUStates::kIOwait_);
  return idle_jiffies;
}

// TODO: Read and return the total number of processes
int LinuxParser::TotalProcesses() {
  string line, key;
  int value{0};
  std::ifstream stream(kProcDirectory + kStatFilename);
  if (stream.is_open()) {
    while (std::getline(stream, line)) {
      std::istringstream linestream(line);
      while (linestream >> key >> value) {
        if (key == "processes") return value;
      }
    }
  }
  return value;
}

// TODO: Read and return the number of running processes
int LinuxParser::RunningProcesses() {
  string line, key;
  int value{0};
  std::ifstream stream(kProcDirectory + kStatFilename);
  if (stream.is_open()) {
    while (std::getline(stream, line)) {
      std::istringstream linestream(line);
      while (linestream >> key >> value) {
        if (key == "procs_running") return value;
      }
    }
  }
  return value;
}

// TODO: Read and return the command associated with a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::Command(int pid) {
  string line;
  std::ifstream stream(kProcDirectory + to_string(pid) + kCmdlineFilename);
  if (stream.is_open()) {
    std::getline(stream, line);
    return line;
  }
  return line;
}

// TODO: Read and return the memory used by a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::Ram(int pid) {
  const int convert_to_mb = 1000;
  double value{0.0};
  string line, key;
  std::ifstream stream(kProcDirectory + to_string(pid) + kStatusFilename);
  if (stream.is_open()) {
    while (std::getline(stream, line)) {
      std::istringstream linestream(line);
      while (linestream >> key >> value) {
        if (key == "VmSize:") value /= convert_to_mb;
      }
    }
    return to_string(value);
  }
  return to_string(value);
}

// TODO: Read and return the user ID associated with a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::Uid(int pid) {
  string line, key;
  double value{0.0};
  std::ifstream stream(kProcDirectory + to_string(pid) + kStatusFilename);
  if (stream.is_open()) {
    while (std::getline(stream, line)) {
      std::istringstream linestream(line);
      while (linestream >> key >> value) {
        if (key == "Uid:") return to_string(value);
      }
    }
  }
  return to_string(value);
}

// TODO: Read and return the user associated with a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::User(int pid) {
  string uid = Uid(pid);
  string line, key, value{};
  std::ifstream stream(kPasswordPath);
  if (stream.is_open()) {
    while (std::getline(stream, line)) {
      std::replace(line.begin(), line.end(), ':', ' ');
      std::replace(line.begin(), line.end(), 'x', ' ');
      std::istringstream linestream(line);
      while (linestream >> value >> key) {
        if (key == uid) {
          return value;
        }
      }
    }
  }
  return value;
}

// TODO: Read and return the uptime of a process
// REMOVE: [[maybe_unused]] once you define the function
long LinuxParser::UpTime(int pid) {
  string line, key;
  double value{0.0};
  std::ifstream stream(kProcDirectory + to_string(pid) + kStatFilename);
  if (stream.is_open()) {
    std::getline(stream, line);
    std::istringstream linestream(line);
    for (int i{0}; i < 21; ++i) {
      linestream >> key;
    }
    linestream >> value;
    value /= sysconf(_SC_CLK_TCK);
    return value;
  }
  return value;
}