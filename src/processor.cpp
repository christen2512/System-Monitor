#include "processor.h"
#include "linux_parser.h"

using namespace LinuxParser;
using std::array;
// TODO: Return the aggregate CPU utilization
void Processor::setJiffies(array<long, 3>& jiffies){
      jiffies[CpuTime::kIdleJiffies] = LinuxParser::IdleJiffies();
      jiffies[CpuTime::kActiveJiffies] = LinuxParser::ActiveJiffies();
      jiffies[CpuTime::kTotalJiffies] = LinuxParser::Jiffies();
}
float Processor::Utilization() { 
 long PrevTotal = jiffies[CpuTime::kIdleJiffies] + jiffies[CpuTime::kActiveJiffies];
 long PrevIdle = jiffies[CpuTime::kIdleJiffies];
 setJiffies(jiffies);
 long CurrentTotal = jiffies[CpuTime::kIdleJiffies] + jiffies[CpuTime::kActiveJiffies];
 long CurrentIdle = jiffies[CpuTime::kIdleJiffies];
 float Total = CurrentTotal - PrevTotal;
 float Idle = CurrentIdle - PrevIdle;
  float usage{0.0f};
 if (Total > 0){
     usage = (Total - Idle) / Total;
 }
 return usage;
}

void Processor::DeltaTime(array<long, 3>& current_jiffies, array<long, 3>& prev_jiffies){
    for(int i{0}; i < 3; ++i)
        delta[i] = current_jiffies[i] - prev_jiffies[i];
}
void Processor::UpdateJiffies(){
     for(size_t i{0}; i < current_jiffies.size(); ++i)
        prev_jiffies[i] = current_jiffies[i];
}
