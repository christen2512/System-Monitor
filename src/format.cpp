#include <string>

#include "format.h"

using std::string;
using std::to_string;

// TODO: Complete this helper function
// INPUT: Long int measuring seconds
// OUTPUT: HH:MM:SS
// REMOVE: [[maybe_unused]] once you define the function
string Format::Convertor(int number){
    if(number < 10)
        return '0' + to_string(number);
    return to_string(number);
}


string Format::ElapsedTime(long seconds) { 
 const int seconds_in_minutes = 60;
 const int seconds_in_hours = 3600;
 const int hours = seconds / seconds_in_hours;
 const int minutes = seconds_in_minutes / (seconds % seconds_in_hours);
 const int remainder = seconds % seconds_in_minutes;
 return Convertor(hours) + ':' + Convertor(minutes) + ':' + Convertor(remainder);
 }