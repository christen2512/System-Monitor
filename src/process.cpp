#include <unistd.h>
#include <cctype>
#include <sstream>
#include <string>
#include <vector>

#include "process.h"
#include "linux_parser.h"
#include "system.h"

using namespace LinuxParser;
using std::string;
using std::to_string;
using std::vector;

// TODO: Return this process's ID
int Process::Pid() {return pid_; }

// TODO: Return this process's CPU utilization
float Process::CpuUtilization() { 
  long int value;
  string line, key;
  vector<double> jiffies_data{0.0};
  std::ifstream stream(kProcDirectory + to_string(pid_) + kStatFilename);
  if (stream.is_open()) {
    std::getline(stream, line);
    std::istringstream linestream(line);
    for (int i{0}; i < 14; ++i) linestream >> key;
    for (int i{0}; i < 5; ++i) {
      linestream >> value;
      jiffies_data.push_back(value);
    }
    for (int i{0}; i < 5; ++i) 
        linestream >> key;
    linestream >> value;
    jiffies_data.push_back(value);
  }
  double Hertz = sysconf(_SC_CLK_TCK);
  double total_time = jiffies_data.at(PidStat::kUtime) + jiffies_data.at(PidStat::kStime);
  double seconds = LinuxParser::UpTime() - (jiffies_data.at(PidStat::kStarttime) / Hertz);
  cpu_usage_ = 100 * ((total_time / Hertz) / seconds );
  return cpu_usage_;
}
// TODO: Return the command that generated this process
string Process::Command() { return LinuxParser::Command(pid_); }

// TODO: Return this process's memory utilization
string Process::Ram() { return LinuxParser::Ram(pid_); }

// TODO: Return the user (name) that generated this process
string Process::User() { return LinuxParser::User(pid_); }

// TODO: Return the age of this process (in seconds)
long int Process::UpTime() { return LinuxParser::UpTime(pid_); }

// TODO: Overload the "less than" comparison operator for Process objects
// REMOVE: [[maybe_unused]] once you define the function
bool Process::operator<(Process const& a) const { return (this->cpu_usage_ > a.cpu_usage_)? true : false; }