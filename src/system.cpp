#include <unistd.h>
#include <cstddef>
#include <set>
#include <string>
#include <vector>
#include<iostream>

#include "process.h"
#include "processor.h"
#include "system.h"
#include "linux_parser.h"

using std::set;
using std::size_t;
using std::string;
using std::vector;

// TODO: Return the system's CPU
Processor& System::Cpu() { return cpu_; }

// TODO: Return a container composed of the system's processes
vector<Process>& System::Processes()
{
  std::vector<int> pids = LinuxParser::Pids();
  for (int pid : pids)
  {
    Process process;
    process.Process::SetCurrentProcess(pid);
    process.Process::CpuUtilization(pid);
    processes_.emplace_back(process);
  }
  
  std::sort(processes_.rbegin(), processes_.rend()); //sort uses defalt less that operator 
  return processes_;	
}

// TODO: Return the system's kernel identifier (string)
std::string System::Kernel()
{
    System::kernel = LinuxParser::Kernel();
    return System::kernel;
}

// TODO: Return the system's memory utilization
float System::MemoryUtilization()
{
  return LinuxParser::MemoryUtilization();
  //return 0.0;
}

// TODO: Return the operating system name
std::string System::OperatingSystem()
{
  return LinuxParser::OperatingSystem();
}

// TODO: Return the number of processes actively running on the system
int System::RunningProcesses()
{
  return LinuxParser::RunningProcesses();
}

// TODO: Return the total number of processes on the system
int System::TotalProcesses()
{
    return LinuxParser::TotalProcesses();
}

// TODO: Return the number of seconds since the system started running
long System::UpTime()
{
  //in seconds
  return LinuxParser::UpTime();
}
 