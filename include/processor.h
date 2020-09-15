#ifndef PROCESSOR_H
#define PROCESSOR_H

#include "linux_parser.h"

using std::array;
using namespace LinuxParser;
using std::size_t;

class Processor {
 public:
  Processor(){
      setJiffies(jiffies);
  }
  float Utilization();  // TODO: See src/processor.cpp
  // TODO: Declare any necessary private members
 private:
    void setJiffies(array<long, 3>&);
    void CurrentJiffies(array<long, 3>&);
    void DeltaTime(array<long, 3>& , array<long, 3>&);
    float Usage();
    void UpdateJiffies();
    array<long, 3> current_jiffies;
    array<long, 3> prev_jiffies;
    array<long, 3> jiffies;
    array<long ,3> delta;
};

#endif