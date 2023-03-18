#include "Tools.h"

using namespace LevelAPI;

std::vector<Tools::LevelRange*> Tools::rangeList;

Tools::LevelRange::LevelRange(int min, int max, std::string *ver) {
    m_nMin = min;
    m_nMax = max;
    m_sGDVer = ver;
}

void Tools::LevelRange::setup() {
    Tools::rangeList = std::vector<Tools::LevelRange *>();
    Tools::rangeList.push_back(new LevelRange(91, 1941, new std::string("1.0")));
    Tools::rangeList.push_back(new LevelRange(1942, 10043, new std::string("1.1")));
    Tools::rangeList.push_back(new LevelRange(10044, 63415, new std::string("1.2")));
    Tools::rangeList.push_back(new LevelRange(63416, 121068, new std::string("1.3")));
    Tools::rangeList.push_back(new LevelRange(121069, 184425, new std::string("1.4")));
    Tools::rangeList.push_back(new LevelRange(184426, 420780, new std::string("1.5")));
    Tools::rangeList.push_back(new LevelRange(420781, 827308, new std::string("1.6")));
    Tools::rangeList.push_back(new LevelRange(827309, 1627362, new std::string("1.7")));
    Tools::rangeList.push_back(new LevelRange(1627363, 2810918, new std::string("1.8")));
    Tools::rangeList.push_back(new LevelRange(2810919, 11020426, new std::string("1.9")));
    Tools::rangeList.push_back(new LevelRange(11020427, 28356225, new std::string("2.0")));
    Tools::rangeList.push_back(new LevelRange(28356226, 100000000, new std::string("2.1")));
    Tools::rangeList.push_back(new LevelRange(100000001, 108000002, new std::string("2.2")));
}
