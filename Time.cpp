#include "Time.h"

#include <chrono>
#include <iomanip>
#include <iostream>
#include <string>
#include <sys/types.h>
#include <vector>

using namespace LevelAPI::Frontend;

Time::Time() {
    using namespace std::chrono;

    // get current time
    auto now = system_clock::now();

    // get number of milliseconds for the current second
    // (remainder after division into seconds)
    auto ms = duration_cast<milliseconds>(now.time_since_epoch()) % 1000;

    // convert to std::time_t in order to convert to std::tm (broken time)
    auto timer = system_clock::to_time_t(now);

    // convert to broken time
    std::tm bt = *std::localtime(&timer);

    this->time_h = bt.tm_hour;
    this->time_m = bt.tm_min;
    this->time_s = bt.tm_sec;

    std::ostringstream oss;

    std::vector<std::string> months = {
        "Jan", "Feb", "Mar", "Apr",
        "May", "Jun", "Jul", "Aug",
        "Sep", "Oct", "Nov", "Dec" 
    };
    time_hms = months[bt.tm_mon - 1] + " " + std::to_string(bt.tm_mday) + "th " + std::to_string(bt.tm_year + 1900) + " at ";

    oss << std::put_time(&bt, "%H:%M:%S"); // HH:MM:SS
    oss << '.' << std::setfill('0') << std::setw(3) << ms.count();

    this->time_hms += oss.str();
}

Time *Time::create() {
    return new Time();
}
std::string Time::getAsString() {
    return this->time_hms;
}
uint64_t Time::getAsInt64() {
    uint64_t h = time_h * 60 * 60;
    uint64_t m = time_m * 60;
    uint64_t s = time_s * 1;
    return h + m + s;
}