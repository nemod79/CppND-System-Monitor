#include <unistd.h>
#include <cctype>
#include <sstream>
#include <string>
#include <vector>

#include "process.h"

using std::string;
using std::to_string;
using std::vector;

Process::Process(int pid) : pid_(pid) {
    user_ = LinuxParser::User(pid_);
    command_ = LinuxParser::Command(pid_);
    cpuutilization_ = 1.0 * LinuxParser::ActiveJiffies(pid_) / sysconf(_SC_CLK_TCK) / LinuxParser::UpTime(pid_);
    ram_ = LinuxParser::Ram(pid_);
}

int Process::Pid() { return pid_; }

float Process::CpuUtilization() { 
  return cpuutilization_;
}

string Process::Command() { return command_; }

string Process::Ram() { return ram_; }

string Process::User() { return user_; }

long int Process::UpTime() { return LinuxParser::UpTime(pid_); }

bool Process::operator<(Process const& a) const {
  if (std::stol(ram_) < std::stol(a.ram_)) return true;
  else return false;
}