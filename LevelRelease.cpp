#include "lapi_database.h"
#include "Tools.h"

using namespace LevelAPI::DatabaseController;

std::string LevelRelease::determineFromID(int id) {
    int i = 0;
    while(i < LevelAPI::Tools::rangeList.size()) {
        if((id >= LevelAPI::Tools::rangeList[i].m_nMin) && (id <= LevelAPI::Tools::rangeList[i].m_nMax)) {
            return LevelAPI::Tools::rangeList[i].m_sGDVer;
        }
        i++;
    }
    return "1.0";
}