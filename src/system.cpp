#include <unistd.h>
#include <cstddef>
#include <set>
#include <string>
#include <vector>

#include "process.h"
#include "processor.h"
#include "system.h"
#include <algorithm>

using std::set;
using std::size_t;
using std::string;
using std::vector;

System::System() {
  kernel_ = LinuxParser::Kernel();
  os_ = LinuxParser::OperatingSystem();
}    
// TODO: Return the system's CPU
Processor& System::Cpu() { return cpu_; }

vector<Process>& System::Processes() { 
  processes_.clear();
  for (int proc_id : LinuxParser::Pids()) {
    processes_.push_back(Process(proc_id));
  }
  std::sort(processes_.begin(),processes_.end());
  // order processes_ vector according to overloaded operator 
  return processes_; 
}

std::string System::Kernel() { 
  return kernel_; 
}

float System::MemoryUtilization() { 
    return LinuxParser::MemoryUtilization(); 
}

std::string System::OperatingSystem() { 
    return os_; 
}

int System::RunningProcesses() { 
    return LinuxParser::RunningProcesses(); 
}

int System::TotalProcesses() { 
    return LinuxParser::TotalProcesses(); 
}

long System::UpTime() { 
    return LinuxParser::UpTime(); 
}