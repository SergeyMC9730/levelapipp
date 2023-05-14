#pragma once

#include <string>
#include <vector>

namespace LevelAPI {
    class LevelRange {
    public:
        int m_nMin;
        int m_nMax;
        std::string m_sGDVer;

        LevelRange(int min, int max, std::string ver);
        int getLevelsInRange();
        int getGV();
    };
}