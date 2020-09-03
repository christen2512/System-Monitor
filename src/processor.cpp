#include "processor.h"
#include "linux_parser.h"

using namespace LinuxParser;
using std::array;
// TODO: Return the aggregate CPU utilization
Processor::Processor() {setJiffies(prev_jiffies);}

float Processor::Utilization() { 
 setJiffies(current_jiffies);
 DeltaTime(current_jiffies, prev_jiffies);   
 float utilization = Usage();
 current_jiffies.swap(prev_jiffies);
 return utilization; }

void setJiffies(array<long, 3>& jiffies){
      jiffies[CpuTime::kIdleJiffies] = LinuxParser::IdleJiffies();
      jiffies[CpuTime::kActiveJiffies] = LinuxParser::ActiveJiffies();
      jiffies[CpuTime::kTotalJiffies] = LinuxParser::Jiffies();
}
void Processor::DeltaTime(array<long, 3>& current_jiffies, array<long, 3>& prev_jiffies){
    for(int i{0}; i < 3; ++i)
        delta[i] = current_jiffies[i] - prev_jiffies[i];
}
float Processor::Usage(){
    const float buffer = 0.0000000001;
    float usage = delta[CpuTime::kActiveJiffies] / (delta[CpuTime::kTotalJiffies + buffer]);
    return usage;
}
void Processor::UpdateJiffies(){
     for(size_t i{0}; i < current_jiffies.size(); ++i)
        prev_jiffies[i] = current_jiffies[i];
}
