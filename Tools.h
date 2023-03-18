#pragma once

#include <string>
#include <vector>

namespace LevelAPI {
    namespace Tools {
        class LevelRange {
        public:
            int m_nMin;
            int m_nMax;
            std::string *m_sGDVer;

            LevelRange(int min, int max, std::string *ver);

            static void setup();
        };

        extern std::vector<LevelRange *> rangeList;
    }
}