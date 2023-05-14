#include "LevelRange.h"

using namespace LevelAPI;

LevelRange::LevelRange(int min, int max, std::string ver) {
    m_nMin = min;
    m_nMax = max;
    m_sGDVer = ver;
}

int LevelRange::getLevelsInRange() {
    return m_nMax - m_nMin;
}

int LevelRange::getGV() {
    float gv = std::stof(m_sGDVer);
    return (int)(gv * 10.f);
}