#pragma once

#include <string>

enum GDPSActionType {
    GATRate = 0,
    GATFeature, GATEpic, GATGodlike, GATLegendary,
    GATCoins
};

class GDPSActionLog {
public:
    std::string m_sModerator;
    GDPSActionType m_eType;
    std::string m_sValue1;
    std::string m_sValue2;
    int m_nLevelID;
    std::string m_sTimestamp;
};