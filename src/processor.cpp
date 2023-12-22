#include "processor.h"
#include "linux_parser.h"

// TODO: Return the aggregate CPU utilization
float Processor::Utilization()
{    
  //Δ active time units / Δ total time units
  float totalTime = LinuxParser::Jiffies();
  float activeTime = LinuxParser::ActiveJiffies();

  float result = (activeTime / totalTime);

  return static_cast<float> (result);  
}