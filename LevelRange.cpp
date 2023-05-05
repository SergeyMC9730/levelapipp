#include "Tools.h"

using namespace LevelAPI;

std::vector<Tools::LevelRange*> Tools::rangeList;

Tools::LevelRange::LevelRange(int min, int max, std::string ver) {
    m_nMin = min;
    m_nMax = max;
    m_sGDVer = ver;
}