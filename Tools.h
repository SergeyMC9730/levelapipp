#pragma once

#include <string>
#include <vector>

namespace LevelAPI {
    namespace Tools {
        void filterString(std::string *str);
        class LevelRange {
        public:
            int m_nMin;
            int m_nMax;
            std::string *m_sGDVer;

            LevelRange(int min, int max, std::string *ver);
        };

        extern std::vector<LevelRange *> rangeList;
    }
}