#pragma once

#include <string>

enum SearchSort {
    SSMostLiked = 0,
    SSMostDownloaded,
    SSNone
};
enum FilterEnum {
    FEByLevelID = 0,
    FEByUserID, FEByAccountID, FEByNickname, FEByName, FEByDescription,
    FENone
};

class SearchFilter {
public:
    int m_nDifficulty = -1;
    int m_nStars = -1;
    SearchSort m_eSort = SSNone;
    FilterEnum m_eFilter = FENone;
    std::string m_sStr;
    std::string m_sSortStr;
    std::string m_sFilterStr;
};