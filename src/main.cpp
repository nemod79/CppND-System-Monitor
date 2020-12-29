#include "ncurses_display.h"
#include "system.h"
#include <iostream>
int main() {
  System system;
  std::vector<Process> test = system.Processes();

//  Process dummy(30);
  //std::cout << test[10].Pid() << "\n" << test[10].User() << "\n" << test[10].Command();
  NCursesDisplay::Display(system);
  //std::cout << LinuxParser::ActiveJiffies() << "\n" << LinuxParser::IdleJiffies() << "\n" << LinuxParser::ActiveJiffies(30) << "\n";
}