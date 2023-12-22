#include <string>
#include <iomanip>
#include <sstream>

#include "format.h"

using std::string;

// TODO: Complete this helper function
// INPUT: Long int measuring seconds
// OUTPUT: HH:MM:SS
// REMOVE: [[maybe_unused]] once you define the function
string Format::ElapsedTime(long seconds)
{
    long HH = seconds / 3600;
    long remSeconds = seconds % 3600;
    long MM = remSeconds / 60;
    long SS = remSeconds % 60;

    std::stringstream time;
    time << std::setfill('0') << std::setw(2) << HH << ":";
    time << std::setfill('0') << std::setw(2) << MM << ":";
    time << std::setfill('0') << std::setw(2) << SS;

    return time.str();
}
