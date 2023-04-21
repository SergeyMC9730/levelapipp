#include "Time.h"

#include <chrono>
#include <iomanip>
#include <iostream>
#include <string>
#include <sys/types.h>
#include <vector>

#include "Translation.h"

using namespace LevelAPI::Frontend;

Time::Time() {
    using namespace std::chrono;

    auto now = system_clock::now();
    auto ms = duration_cast<milliseconds>(now.time_since_epoch()) % 1000;
    auto timer = system_clock::to_time_t(now);

    std::tm bt = *std::localtime(&timer);

    std::ostringstream oss;

    auto timeorder = Translation::getByKey("lapi.time.dateorder");
    auto dayformat = Translation::getByKey("lapi.time.dayformat");
    auto timepointer = Translation::getByKey("lapi.time.timepointer");
    
    auto jan = Translation::getByKey("lapi.time.jan");
    auto feb = Translation::getByKey("lapi.time.feb");
    auto mar = Translation::getByKey("lapi.time.mar");
    auto apr = Translation::getByKey("lapi.time.apr");
    auto may = Translation::getByKey("lapi.time.may");
    auto jun = Translation::getByKey("lapi.time.jun");
    auto jul = Translation::getByKey("lapi.time.jul");
    auto aug = Translation::getByKey("lapi.time.aug");
    auto sep = Translation::getByKey("lapi.time.sep");
    auto oct = Translation::getByKey("lapi.time.oct");
    auto nov = Translation::getByKey("lapi.time.nov");
    auto dec = Translation::getByKey("lapi.time.dec");

    std::vector<std::string> months = {
        jan, feb, mar, apr,
        may, jun, jul, aug,
        sep, oct, nov, dec 
    };

    int i = 0;
    while(i < timeorder.size()) {
        switch(timeorder[i]) {
            case 'm' : {
                time_hms += months[bt.tm_mon] + " ";
                break;
            }
            case 'd': {
                time_hms += std::to_string(bt.tm_mday) + dayformat + " ";
                break;
            }
            case 'y': {
                time_hms += std::to_string(bt.tm_year + 1900) + " ";
                break;
            }
        }
        i++;
    }

    time_hms += timepointer + " ";

    oss << std::put_time(&bt, "%H:%M:%S"); // HH:MM:SS
    oss << '.' << std::setfill('0') << std::setw(3) << ms.count();

    this->time_hms += oss.str();
    this->time_h = bt.tm_hour;
    this->time_m = bt.tm_min;
    this->time_s = bt.tm_sec;
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