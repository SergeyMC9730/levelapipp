/**
 *  LevelAPI - Geometry Dash level cacher with search functionality and more.
    Copyright (C) 2023  Sergei Baigerov

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <https://www.gnu.org/licenses/>.
*/

#include "Time.h"

#include <chrono>
#include <iomanip>
#include <iostream>
#include <string>
#include <sys/types.h>
#include <vector>

#include "Translation.h"

#include "StringSplit.h"

using namespace LevelAPI::Frontend;

Time::Time(uint64_t unixTimestamp) {
    using namespace std::chrono;

    auto t = time(0);

    if (unixTimestamp != 0) {
        t = unixTimestamp;
    }

    auto now = system_clock::from_time_t(t);

    unixTime = t;

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

    bool nine11 = (bt.tm_mday == 11 && bt.tm_mon == 8);

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
		if (nine11) {
			time_hms += "2001 ";
			break;
		}
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

    this->unixTime = std::chrono::duration_cast<std::chrono::seconds>(now.time_since_epoch()).count();
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

void Time::fromTimeString(std::string s) {
    auto timeorder = Translation::getByKey("lapi.time.dateorder");
    auto str = s; // Jul 15th 2023 at 23:04:18.173
    auto stuff = splitString(str.c_str(), ' ');
    
    std::string month;
    int day;
    int year = 0;

    int i = 0;
    while(i < timeorder.size()) {
        switch(timeorder[i]) {
            case 'm' : {
                month = stuff[i];
                break;
            }
            case 'd': {
                day = std::stoi(stuff[i]);
                break;
            }
            case 'y': {
                year = std::stoi(stuff[i]);
                break;
            }
        }
        i++;
    }

    std::string time = stuff[4];

    auto timestuff = splitString(time.c_str(), ':');

    int hour = std::stoi(timestuff[0]);
    int minute = std::stoi(timestuff[1]);
    int second = std::stoi(timestuff[2]);

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

    std::map<std::string, int> monthMap = {
        {jan, 1}, {feb, 2}, {mar, 3},
        {apr, 4}, {may, 5}, {jun, 6},
        {jul, 7}, {aug, 8}, {sep, 9},
        {oct, 10},{nov,11},{dec,12}
    };

    int month_val = monthMap[month];
    
    std::string s2 = std::to_string(year) + "-" + ((month_val < 10) ? "0" : "") + std::to_string(month_val) + "-" + ((day < 10) ? "0" : "") + std::to_string(day) + "T" + ((hour < 10) ? "0" : "") + std::to_string(hour) + ":" + ((minute < 10) ? "0" : "") + std::to_string(minute) + ":" + ((second < 10) ? "0" : "") + std::to_string(second) + ".000Z";
    std::tm t{};
    std::istringstream ss(s2);

    ss >> std::get_time(&t, "%Y-%m-%dT%H:%M:%S");
    if (ss.fail()) {
        throw std::runtime_error{"failed to parse time string"};
    }   
    std::time_t time_stamp = mktime(&t);

    unixTime = time_stamp;
    
    time_h = hour;
    time_m = minute;
    time_s = second;

    time_hms = s;

    return;
}
