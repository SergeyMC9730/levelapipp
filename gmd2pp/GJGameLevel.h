#pragma once

#include <string>
#include <vector>

#define PKSTRING 0
#define PKINT 1
#define PKBOOL 2
#define PKBASE64 3

class GJGameLevel {
public:
    int m_nLevelID;
    int m_nVersion;
    int m_nPlayerID;
    int m_nDownloads;
    int m_nMusicID;
    int m_nLikes;
    int m_nLength;
    int m_nDifficultyDenominator;
    int m_nDifficultyNumerator;
    int m_nSetCompletes;
    int m_nGameVersion;
    int m_nDislikes;
    int m_nStars;
    int m_nFeatureScore;
    int m_nCopiedID;
    int m_nCoins;
    int m_nStarsRequested;
    int m_nDailyNumber;
    int m_nEpic;
    int m_nDemonDifficulty;
    int m_nObjects;
    int m_nEditorTime;
    int m_nEditorTimeTotal;
    int m_nSongID;
    int m_nAccountID;
    int m_nMoons;
    
    bool m_bAuto;
    bool m_bDemon;
    bool m_bVerifiedCoins;
    bool m_bLDM;
    bool m_bGauntlet;
    bool m_b2P;

    std::string m_sLevelName;
    std::string m_sDescription;
    std::string m_sLevelString;
    std::string m_sXORPassword;
    std::string m_sUploadDate;
    std::string m_sUpdateDate;
    std::string m_sExtraString;
    std::string m_sSettings;
    std::string m_sRecordString;
    std::string m_sUsername;

    std::vector<int> m_vSFXList;
};
