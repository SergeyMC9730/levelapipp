#include "Tools.h"

using namespace LevelAPI;

std::vector<Tools::LevelRange> Tools::rangeList;

Tools::LevelRange::LevelRange(int min, int max, std::string ver) {
    m_nMin = min;
    m_nMax = max;
    m_sGDVer = ver;
}

void Tools::LevelRange::setup() {
    Tools::rangeList = std::vector<Tools::LevelRange>();
    Tools::rangeList.push_back(LevelRange(91, 1941, "1.0"));
    Tools::rangeList.push_back(LevelRange(1942, 10043, "1.1"));
    Tools::rangeList.push_back(LevelRange(10044, 63415, "1.2"));
    Tools::rangeList.push_back(LevelRange(63416, 121068, "1.3"));
    Tools::rangeList.push_back(LevelRange(121069, 184425, "1.4"));
    Tools::rangeList.push_back(LevelRange(184426, 420780, "1.5"));
    Tools::rangeList.push_back(LevelRange(420781, 827308, "1.6"));
    Tools::rangeList.push_back(LevelRange(827309, 1627362, "1.7"));
    Tools::rangeList.push_back(LevelRange(1627363, 2810918, "1.8"));
    Tools::rangeList.push_back(LevelRange(2810919, 11020426, "1.9"));
    Tools::rangeList.push_back(LevelRange(11020427, 28356225, "2.0"));
    Tools::rangeList.push_back(LevelRange(28356226, 100000000, "2.1"));
    Tools::rangeList.push_back(LevelRange(100000001, 108000002, "2.2"));
}
