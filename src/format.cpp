#include <string>

#include "format.h"

using std::string;
using std::to_string;

// TODO: Complete this helper function
// INPUT: Long int measuring seconds
// OUTPUT: HH:MM:SS
// REMOVE: [[maybe_unused]] once you define the function
string Format::Convertor(int number){
    if(number < 10){
        if(number < 0 ){
            number += number;
        }
        else 
            return '0' + to_string(number);
    }
    return to_string(number);
}


string Format::ElapsedTime(long seconds) { 
 const int seconds_in_minutes = 60;
 const int seconds_in_hours = 3600;
 int hours{0};
 int minutes{0};
 int remainder{0};
 if(seconds >= seconds_in_hours)
     hours = seconds / seconds_in_hours;
 if(seconds >= seconds_in_minutes && seconds - seconds_in_hours < seconds_in_minutes)
    minutes = seconds / seconds_in_minutes;
 else if(seconds >= seconds_in_minutes && (seconds - seconds_in_hours) >= seconds_in_minutes)
    minutes = (seconds - seconds_in_hours) / seconds_in_minutes;
 if(hours == 0){
    if(minutes == 0){
        remainder = seconds;
    }
    else{
        remainder = seconds % seconds_in_minutes;
    }
 }
 else{
     remainder = (seconds - seconds_in_hours) - (minutes * seconds_in_minutes);
 }
 if(minutes >= 60 ){
     hours ++;
     minutes -= 60;
 }
 return Convertor(hours) + ':' + Convertor(minutes) + ':' + Convertor(remainder);
 }