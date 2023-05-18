#pragma once

#include <string>

namespace LevelAPI {
    namespace HttpController {
        void parse();
        void setup();
        void save();
                
        int getPort();
        int getThreads();
        std::string getURL();
    }
}