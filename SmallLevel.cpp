#include "SmallLevel.h"

using namespace LevelAPI::Backend;

SmallLevel::SmallLevel() {}
SmallLevel::SmallLevel(std::string a, std::string b, int c) {
    this->m_sLevelName = a;
    this->m_nLevelID = c;
    this->m_sAuthor = b;
}

bool SmallLevel::operator==(const SmallLevel& rhs) {
    if(rhs.m_nLevelID == this->m_nLevelID && rhs.m_sLevelName == this->m_sLevelName && rhs.m_sAuthor == this->m_sAuthor) return true;
    return false;
}


bool SmallLevel::operator==(const SmallLevel *rhs) {
    if(rhs->m_nLevelID == this->m_nLevelID && rhs->m_sLevelName == this->m_sLevelName && rhs->m_sAuthor == this->m_sAuthor) return true;
    return false;
}