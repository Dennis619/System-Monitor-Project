#include <unistd.h>
#include <cctype>
#include <sstream>
#include <string>
#include <vector>
#include<iostream>

#include "process.h"
#include "linux_parser.h"

using std::string;
using std::to_string;
using std::vector;

//getter
int Process::GetCurrentProcess()const { return currentProcess; }

//setter
void Process::SetCurrentProcess(int pid)
{
  this->currentProcess = pid;
}

//constructor
/*
Process::Process(int pid)
{
  SetCurrentProcess(pid);
}
*/
// TODO: Return this process's ID
int Process::Pid()
{
  return Process::GetCurrentProcess();
}


float Process::CpuUtilization()const
{
  return cpuUtilizationCurrent;
}

void Process::CpuUtilization(int pid)
{
  cpuUtilizationCurrent = LinuxParser::CpuUtilization(pid);
}


// TODO: Return the command that generated this process
string Process::Command()
{
    return LinuxParser::Command(Process::GetCurrentProcess());
}

// TODO: Return this process's memory utilization
string Process::Ram()
{
    return LinuxParser::Ram(Process::GetCurrentProcess());
}

// TODO: Return the user (name) that generated this process
string Process::User()
{
  return LinuxParser::UserName();
}

// TODO: Return the age of this process (in seconds)
long int Process::UpTime()
{
  return LinuxParser::UpTime(Process::GetCurrentProcess());
}

// TODO: Overload the "less than" comparison operator for Process objects
// REMOVE: [[maybe_unused]] once you define the function
bool Process::operator<(Process const& a)const
{
  return a.Process::CpuUtilization() < Process::CpuUtilization();
}
