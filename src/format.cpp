#include <string>

#include "format.h"

using std::string;

// TODO: Complete this helper function
// INPUT: Long int measuring seconds
// OUTPUT: HH:MM:SS
// REMOVE: [[maybe_unused]] once you define the function
string Format::ElapsedTime(long seconds) { 
    int hour_;
    int minutes_;
    int seconds_;
    string outstring;
    hour_ = (seconds / 3600) % 24;
    if (hour_ <= 9) 
        outstring = "0"; 
    outstring += std::to_string(hour_) + ":";
    minutes_ = (seconds / 60) % 60;
    if (minutes_ <= 9) 
        outstring += "0"; 
    outstring += std::to_string(minutes_) + ":";
    seconds_ = seconds % 60;
    if (seconds_ <= 9) 
        outstring += "0"; 
    outstring += std::to_string(seconds_);

    return outstring;
}