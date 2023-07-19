#pragma once

#include <string>

enum SearchSort {
    SSMostLiked = 0,
    SSMostDownloaded,
    SSLatestDBApperead,
    SSRecentLevel,
    SSNone
};

class SearchFilter {
public:
    int m_nDifficulty = -1;
    int m_nStars = -1;

    SearchSort m_eSort = SSNone;

    std::string m_sName = "";
    std::string m_sDescription = "";

    int m_nPage = 1;
    int m_nLevelsPerPage = 10;

    int m_nUID = 0;
    int m_nAID = 0;

    std::string m_sReleaseGV = "";
    int m_nServerGV = 0;

    int m_nSID = -1;
    bool m_bSongOfficial = false;
};