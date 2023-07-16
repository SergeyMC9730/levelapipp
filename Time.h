#pragma once

#include <string>
#include <vector>
#include <cstdint>

namespace LevelAPI {
    namespace Frontend {
        class Time {
        protected:
            std::string time_hms;
            int time_h;
            int time_m;
            int time_s;

            Time();
        public:
            uint64_t unixTime;

            static Time *create();
            void fromTimeString(std::string str);

            std::string getAsString();
            uint64_t getAsInt64();
        };
    }
}