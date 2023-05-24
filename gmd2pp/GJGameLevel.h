#pragma once

#include <string>
#include <vector>

#define PKSTRING 0
#define PKINT 1
#define PKBOOL 2
#define PKBASE64 3

class GJGameLevel {
public:
    int m_nLevelID = 0;
    int m_nVersion = 0;
    int m_nPlayerID = 0;
    int m_nDownloads = 0;
    int m_nMusicID = 0;
    int m_nLikes = 0;
    int m_nLength = 0;
    int m_nDifficultyDenominator = 0;
    int m_nDifficultyNumerator = 0;
    int m_nSetCompletes = 0;
    int m_nGameVersion = 0;
    int m_nDislikes = 0;
    int m_nStars = 0;
    int m_nFeatureScore = 0;
    int m_nCopiedID = 0;
    int m_nCoins = 0;
    int m_nStarsRequested = 0;
    int m_nDailyNumber = 0;
    int m_nEpic = 0;
    int m_nDemonDifficulty = 0;
    int m_nObjects = 0;
    int m_nEditorTime = 0;
    int m_nEditorTimeTotal = 0;
    int m_nSongID = 0;
    int m_nAccountID = 0;
    int m_nMoons = 0;
    
    bool m_bAuto = false;
    bool m_bDemon = false;
    bool m_bVerifiedCoins = false;
    bool m_bLDM = false;
    bool m_bGauntlet = false;
    bool m_b2P = false;

    std::string m_sLevelName = "";
    std::string m_sDescription = "";
    std::string m_sLevelString = "";
    std::string m_sXORPassword = "";
    std::string m_sUploadDate = "";
    std::string m_sUpdateDate = "";
    std::string m_sExtraString = "";
    std::string m_sSettings = "";
    std::string m_sRecordString = "";
    std::string m_sUsername = "";

    std::vector<int> m_vSFXList = {};
};
