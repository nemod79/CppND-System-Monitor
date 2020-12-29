#include "processor.h"

float Processor::Utilization() { 
    float activejiffies = LinuxParser::ActiveJiffies() / 1.0;
    float totaljiffies = LinuxParser::Jiffies() / 1.0;
    return activejiffies / totaljiffies; 
}