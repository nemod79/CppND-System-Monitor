#include <dirent.h>
#include <unistd.h>
#include <string>
#include <vector>

#include "linux_parser.h"

using std::stof;
using std::string;
using std::to_string;
using std::vector;

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
  string os, kernel;
  string line;

  std::ifstream stream(kProcDirectory + kVersionFilename);
  if (stream.is_open()) {
    std::getline(stream, line);
    std::istringstream linestream(line);
    linestream >> os >> kernel;
    linestream >> kernel;
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

// DONE: completed, to be tested
float LinuxParser::MemoryUtilization() { 
  string memtotal, memfree, key, value;
  string line;
  bool parsingcompleted = false;

  std::ifstream stream(kProcDirectory + kMeminfoFilename);
  if (stream.is_open()) {
    while(std::getline(stream,line) && !parsingcompleted) {
      std::istringstream linestream(line);
      if (linestream >> key && key == "MemTotal:")
        linestream >> memtotal;
      else if (key == "MemFree:") {
        linestream >> memfree;
        parsingcompleted = true;
      }
    }
  } 
  return 1.0 - std::stof(memfree) / std::stof(memtotal);
}

// DONE: Completed, TBT
long LinuxParser::UpTime() { 
  string seconds;
  string line;
  std::ifstream stream(kProcDirectory + kUptimeFilename);
  if (stream.is_open()) {
    std::getline(stream,line);
    std::istringstream linestream(line);
    linestream >> seconds;
  }
  return std::stol(seconds); 
}

long LinuxParser::Jiffies() { 
  return ActiveJiffies() + IdleJiffies();
}

long LinuxParser::ActiveJiffies(int pid) { 
  string line;
  string item, dummy;
  long total_time = 0;
  std::ifstream stream(kProcDirectory + std::to_string(pid) + kStatFilename);
  if (stream.is_open()) {
    std::getline(stream,line);
    std::istringstream linestream(line);
    for (int dummy_idx = 1; dummy_idx < 14; dummy_idx++)
      linestream >> dummy;

    for (int idx = 1; idx <= 4; idx++) {
      linestream >> item;
      total_time += std::stol(item);
    }
  }

  return total_time; 
}

long LinuxParser::ActiveJiffies() { 
  vector<string> cputilization = CpuUtilization();
  long activejiffies;

  activejiffies = std::stoi(cputilization[kUser_]) +
                  std::stoi(cputilization[kNice_]) +
                  std::stoi(cputilization[kSystem_]) + 
                  std::stoi(cputilization[kIRQ_]) +
                  std::stoi(cputilization[kSoftIRQ_]) +
                  std::stoi(cputilization[kSteal_]);

  return activejiffies; 
}

long LinuxParser::IdleJiffies() { 
  vector<string> cputilization = CpuUtilization();
  long idlejiffies;
  idlejiffies = std::stoi(cputilization[kIdle_]) +
                std::stoi(cputilization[kIOwait_]);

  return idlejiffies; 
}

vector<string> LinuxParser::CpuUtilization() { 
  string line;
  string key;
  vector<string> output;
  std::ifstream stream(kProcDirectory + kStatFilename);
  if (stream.is_open()) { 
    std::getline(stream,line);
    std::istringstream linestream(line);
    if (linestream >> key && key == "cpu") {
      while (linestream >> key)
        output.push_back(key);
    }    
  }
  return output; 
}

int LinuxParser::TotalProcesses() { 
  string processes;
  string key;
  string line;
  bool parsingcompleted = false;
  std::ifstream stream(kProcDirectory + kStatFilename);
  if (stream.is_open()) {
    while(std::getline(stream,line) && !parsingcompleted) {
      std::istringstream linestream(line);
      if (linestream >> key && key == "processes") {
        linestream >> processes;
        parsingcompleted = true;
      }
    }  
  }  
  return std::stoi(processes); 
}

int LinuxParser::RunningProcesses() {
  string running_proc;
  string key;
  string line;
  bool parsingcompleted = false;
  std::ifstream stream(kProcDirectory + kStatFilename);
  if (stream.is_open()) {
    while(std::getline(stream,line) && !parsingcompleted) {
      std::istringstream linestream(line);
      if (linestream >> key && key == "procs_running") {
        linestream >> running_proc;
        parsingcompleted = true;
      }
    }  
  }  
  return std::stoi(running_proc);
}

string LinuxParser::Command(int pid) { 
  string command;
  std::ifstream stream(kProcDirectory + std::to_string(pid) + kCmdlineFilename);
  if (stream.is_open()) { 
    std::getline(stream,command);
  }  
  return command; 
}

string LinuxParser::Ram(int pid) { 
  string memory;
  string key, line;
  bool parsecompleted = false;
  std::ifstream stream(kProcDirectory + std::to_string(pid) + kStatusFilename);
  if (stream.is_open()) {
    while(std::getline(stream,line) && !parsecompleted) {
      std::istringstream linestream(line);
      if (linestream >> key && key == "VmSize:") {
        linestream >> memory;
        parsecompleted = true;
      }  
    }  
  }
  return std::to_string(std::stol(memory) / 1000);
}

string LinuxParser::Uid(int pid) { 
  string uid;
  string key, line;
  bool parsecompleted = false;
  std::ifstream stream(kProcDirectory + std::to_string(pid) + kStatusFilename);
  if (stream.is_open()) {
    while(std::getline(stream,line) && !parsecompleted) {
      std::istringstream linestream(line);
      if (linestream >> key && key == "Uid:") {
        linestream >> uid;
        parsecompleted = true;
      }  
    }  
  }

  return uid; 
}

string LinuxParser::User(int pid) { 
  string uid = Uid(pid);
  string user;
  string key, dummy; 
  string line;
  bool parsecompleted = false;
  std::ifstream stream(kPasswordPath);
  if (stream.is_open()) {
    while(std::getline(stream,line) && !parsecompleted) {
      std::replace(line.begin(), line.end(), ':',' ');
      std::istringstream linestream(line);
      if (linestream >> user >> dummy >> key)
        if (key == uid)
          parsecompleted = true;
    }  
  }
  return user; 
}

long LinuxParser::UpTime(int pid) { 
  string starttuptime; 
  string line;

  std::ifstream stream(kProcDirectory + std::to_string(pid) + kStatFilename);
  if (stream.is_open()) {
    while(std::getline(stream,line)) {
      std::istringstream linestream(line);
      for(int idx = 1; idx <= 22; idx++)
        linestream >> starttuptime; 
;
    }  
  }

  long proc_uptime = UpTime() - std::stol(starttuptime) / sysconf(_SC_CLK_TCK); 
  return proc_uptime;
}