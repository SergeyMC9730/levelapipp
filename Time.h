#pragma once

#include <string>
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
            static Time *create();

            std::string getAsString();
            uint64_t getAsInt64();
        };
    }
}