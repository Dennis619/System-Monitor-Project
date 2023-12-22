#include <dirent.h>
#include <unistd.h>
#include <string>
#include <vector>
#include<iostream>
#include <cmath>

#include "linux_parser.h"

using std::stof;
using std::string;
using std::to_string;
using std::vector;

long LinuxParser::HZ()
{
  return sysconf(_SC_CLK_TCK);
}

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
  string os, kernel, version;
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

// TODO: Read and return the system memory utilization
float LinuxParser::MemoryUtilization()
{
  //MemTotal-MemFree
  string line;
  double totalMem = 0.0;
  double freeMem = 0.0;
  double buffers = 0.0;
  double cached = 0.0;
  float utilization = 0.0;
  
  std::ifstream stream(kProcDirectory + kMeminfoFilename);
  if(stream.is_open())
  {
    while(std::getline(stream, line))
    {
      string memT,memF, buff, cach, noUse;
      std::istringstream linestream(line);
      if(line.find("MemTotal:"))
      {
        linestream >> noUse >> memT;
        totalMem = std::stod(memT);
      }
      if(line.find("MemFree:"))
      {
        std::istringstream linestream(line);
        linestream >> noUse >> memF;
        freeMem = std::stod(memF);          
      }
      if(line.find("Buffers:"))
      {
        std::istringstream linestream(line);
        linestream >> noUse >> buff;
        buffers = std::stod(buff);          
      }
      if(line.find("Cached:"))
      {
        std::istringstream linestream(line);
        linestream >> noUse >> cach;
        cached = std::stod(cach);          
      }
    }
         stream.close();
  }
  // Calculate memory utilization
  //utilization = (100 * (static_cast<float>(freeMem)) / static_cast<float>(totalMem));
  utilization = ((static_cast<float>(totalMem) - static_cast<float>(freeMem) - (static_cast<float>(cached) + static_cast<float>(buffers))) / static_cast<float>(totalMem));
         
  return (std::abs(utilization))/100;
}

// TODO: Read and return the system uptime
long LinuxParser::UpTime()
{ 
  string uptime;
  string line;
  long time = 0;
  std::ifstream stream(kProcDirectory + kUptimeFilename);
  if (stream.is_open()) {
    std::getline(stream, line);
    std::istringstream linestream(line);
    linestream >> uptime;
    if(uptime.empty())
    {
      time = -1;
    }else
    {
      time = stol(uptime)/ LinuxParser::HZ();
    }
    stream.close();    
  }  
  return time;
}

// TODO: Read and return the number of jiffies for the system
long LinuxParser::Jiffies()
{
  long jiffiesTotal = 0;
  string line;

  std::ifstream stream(kProcDirectory + kStatFilename);
  if(stream.is_open())
  {
    string noUse, user, nice, system, idle, iowait, irq, softirq, steal, guest, guest_nice;
    while(std::getline(stream, line))
    {      
      //string
      std::istringstream linestream(line); 
      if(line.find("cpu"))
      {
        linestream >> noUse >> user >> nice >> system >> idle >> iowait>>
         irq >> softirq >> steal >> guest >> guest_nice;

        //calculate jiffies
        if(user.empty() && nice.empty() && system.empty() && idle.empty() && iowait.empty() &&
        irq.empty() && softirq.empty() && steal.empty() && guest.empty() && guest_nice.empty())
        {
          jiffiesTotal = -1;
        }else
        {
          jiffiesTotal = stol(user) + stol(nice)+ stol(system)+ stol(idle)+ stol(iowait)+ 
            stol(irq) + stol(softirq) + stol(steal)+ stol(guest) + stol(guest_nice);
        }      
      }
    }
    stream.close();
  }
  return jiffiesTotal;
}

// TODO: Read and return the number of active jiffies for a PID
// REMOVE: [[maybe_unused]] once you define the function
long LinuxParser::ActiveJiffies(int pid)
{
  pid = 0;
  return pid;
}

// TODO: Read and return the number of active jiffies for the system
long LinuxParser::ActiveJiffies()
{
  long jiffiesTotalActive = 0;
  string line;

  std::ifstream stream(kProcDirectory + kStatFilename);
  if(stream.is_open())
  {
    string noUse, user, nice, system, idle, iowait, irq, softirq, steal, guest, guest_nice;
    while(std::getline(stream, line))
    {      
      //string 
      std::istringstream linestream(line);
      if(line.find("cpu"))
      {
        linestream >> noUse >> user >> nice >> system >> idle >> iowait>>
         irq >> softirq >> steal >> guest >> guest_nice;

        //calculate jiffies
        if(user.empty() && nice.empty() && system.empty() && idle.empty() && iowait.empty()
         && irq.empty() && softirq.empty() && steal.empty() && guest.empty() && guest_nice.empty())
         {
          jiffiesTotalActive = -1;
         }
         else{
          jiffiesTotalActive = stol(user) + stol(nice)+ stol(system)+ 
            stol(irq) + stol(softirq) + stol(steal);
         }
      }
    }
    stream.close();
  }
  return jiffiesTotalActive;
}

// TODO: Read and return the number of idle jiffies for the system
long LinuxParser::IdleJiffies()
{
  long idleJiffies = 0;
  string line;

  std::ifstream stream(kProcDirectory + kStatFilename);
  if(stream.is_open())
  {
    string noUse, user, nice, system, idle, iowait, irq, softirq, steal, guest, guest_nice;
    std::istringstream linestream(line);
    while(std::getline(stream, line))
    {      
      //string 
      if(line.find("cpu"))
      {
        linestream >> noUse >> user >> nice >> system >> idle >> iowait>>
         irq >> softirq >> steal >> guest >> guest_nice;

        //calculate jiffies
        idleJiffies += stol(idle);
        break;
      }
    }
    stream.close();
  }
  return idleJiffies;
}

// TODO: Read and return CPU utilization
vector<string> LinuxParser::CpuUtilization() { return {}; }

// TODO: Read and return the total number of processes
int LinuxParser::TotalProcesses()
{
  string ProcessesRunning;
  string line;
  long number = 0;

  std::ifstream stream(kProcDirectory + kStatFilename);
  if(stream.is_open())
  {    
    while(std::getline(stream, line))
    {
      if(line.find("processes"))
      {
        string noUse;
        std::istringstream linestream(line);
        linestream >> noUse >> ProcessesRunning;
        if(ProcessesRunning.empty())
        {
          number = -1;
        }else
        {
          number = std::stol(ProcessesRunning);
        }
      }
    }
    stream.close();
  }
  return static_cast<int>(number);
}

// TODO: Read and return the number of running processes
int LinuxParser::RunningProcesses()
{
  string numberProcessesRunning;
  string line, noUse;
  long number = 0;

  std::ifstream stream(kProcDirectory + kStatFilename);
  if(stream.is_open())
  {    
    while(std::getline(stream, line))
    {
      std::istringstream linestream(line);
      linestream >> noUse >> numberProcessesRunning;
      if(noUse == "procs_running")
      {
        number = std::stol(numberProcessesRunning);
      }
    }
    stream.close();
  }
  return static_cast<int>(number);
}

// TODO: Read and return the command associated with a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::Command(int pid)
{
  string line;
  std::ifstream stream(kProcDirectory + std::to_string(pid) + kCmdlineFilename);
  if(stream.is_open())
  {
    getline(stream, line);
    stream.close();
  }
  return line;  
}

//return user name
string LinuxParser::UserName()
{
  string userName;
  string line;
  std::vector<string> parts;

  std::ifstream stream(kPasswordPath);
  if(stream.is_open())
  {
    std::istringstream linestream(line);

    while(std::getline(stream, line, ':'))
    {
      parts.push_back(line);
    }
    userName = parts[0];
    stream.close();
  }
  return userName;
}

// TODO: Read and return the memory used by a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::Ram(int pid)
{
  string ram;
  long size = 0;
  string line;

  std::ifstream stream(kProcDirectory + std::to_string(pid) + kStatusFilename);
  if(stream.is_open())
  {
    while(std::getline(stream, line))
    {
        string noUse;
        std::istringstream linestream(line);
        linestream >> noUse >> ram;

        if(noUse == "VmSize:")
        {
          if(ram.empty())
          {
            size = -1;
          }
          else
          {
            size = stol(ram) / 1000;
          }
        }
      
    }
    stream.close();   
  }
  return std::to_string(size);
}

// TODO: Read and return the user ID associated with a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::Uid(int pid)
{
  string userID;
  string line;

  std::ifstream stream(kProcDirectory + std::to_string(pid) + kStatusFilename);
  if(stream.is_open())
  {
    std::istringstream linestream(line);
    while(std::getline(stream, line))
    {
      //string 
      if(line.find("Uid:"))
      {
        string noUse;
        linestream >> noUse >> userID;
      }
    }
    stream.close();
  }
  return userID;
} 

// TODO: Read and return the user associated with a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::User(int pid)
{
  string userName;
  string line;

  std::ifstream stream(kProcDirectory + std::to_string(pid) + kStatusFilename);
  if(stream.is_open())
  {
    std::istringstream linestream(line);
    while(std::getline(stream, line))
    {
      //string 
      if(line.find("Name:"))
      {
        string noUse;
        linestream >> noUse >> userName;
      }
    }
    stream.close();
  }
  return userName;
} 

//cpu usage information
float LinuxParser::CpuUtilization(int pid)
{
  float cpuUsage = 0.0;
  double totalTime = 0.0;
  double seconds = 0.0;
  long startTime = LinuxParser::UpTime();
  string line, token, uptime, stime, cutime, cstime;
  std::ifstream stream(kProcDirectory + std::to_string(pid) + kStatFilename);
  if(stream.is_open())
  {
    while (std::getline(stream, line))
    {
      std::istringstream linestream(line);
      //process start time
      for(int i = 1; i < 18; i++)
      {
        
        //add 14 to 17
        linestream >> token;
        switch (i)
        {
          case 14:
            uptime = token;
            break;
          case 15:
            stime = token;
            break;
          case 16:
          cutime = token;
          break;
          case 17:
          cstime = token;
          break;
        }
      }
    }  

    if(uptime.empty() && stime.empty() && cstime.empty() && cstime.empty())
      {
        totalTime = -1;
      }
      else
      {
        totalTime = std::stod(uptime) + std::stod(cstime) + std::stod(stime) + std::stod(cutime);
        seconds = std::stod(uptime) - ((float)startTime / (float)LinuxParser::HZ());
      }     

      cpuUsage = ((float)totalTime / (float)LinuxParser::HZ()) / (float)seconds;
  }
  stream.close();
  return std::abs(cpuUsage);
}

// TODO: Read and return the uptime of a process
// REMOVE: [[maybe_unused]] once you define the function
long LinuxParser::UpTime(int pid)
{ 
  string uptime;
  string line;
  long time = 0;
  std::ifstream stream(kProcDirectory + std::to_string(pid) + kStatFilename);
  
  if (stream.is_open()) {
    std::getline(stream, line);
    std::istringstream linestream(line);
    //process start time is 22
    for(int i = 1; i <= 22; i++)
    {
      if(i == 22){
        linestream >> uptime;
      }
    }
    if(uptime.empty())
    {
      time = -1;
    }else
    {
      time = stol(uptime);
    }
    stream.close();    
  }  
  return time;
}




