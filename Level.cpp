#include "lapi_database.h"
#include "Tools.h"
#include "json/single_include/nlohmann/json.hpp"
#include <fstream>

using namespace LevelAPI::DatabaseController;

Level::Level() {
    m_uRelease = new LevelRelease();
    m_uRelease->m_fActualVersion = new std::string(m_uRelease->determineFromID(m_nLevelID));

    setupJSON();
}

void Level::setupJSON() {
    levelJson = nlohmann::json();
}

void Level::save() {
    #define fill(str, val) levelJson[str] = val;
    #define fill_str(str, val) fill(str, val->c_str());

    m_uRelease->m_fActualVersion = new std::string(m_uRelease->determineFromID(m_nLevelID));

    fill("levelID", m_nLevelID)
    fill("version", m_nVersion)
    fill("playerID", m_nPlayerID)
    fill("accountID", m_nAccountID)
    fill("downloads", m_nDownloads)
    fill("musicID", m_nMusicID)
    fill("likes", m_nLikes)
    fill("length", m_nLength)
    fill("difficulty_denominator", m_nDifficultyDenominator)
    fill("difficulty_numerator", m_nDifficultyNumerator)
    fill("fakeGameVersion", m_nGameVersion)
    fill("dislikes", m_nDislikes)
    fill("stars", m_nStars)
    fill("featureScore", m_nFeatureScore)
    fill("copiedFrom", m_nCopiedID)
    fill("dailyNumber", m_nDailyNumber)
    fill("coins", m_nCoins)
    fill("starsReequested", m_nStarsRequested)
    fill("isEpic", (bool)m_nEpic)
    fill("demonDifficulty", m_nDemonDifficulty)
    fill("editorTime", m_nEditorTime)
    fill("editorTimeTotal", m_nEditorTimeTotal)
    fill("songID", m_nSongID)

    fill("isAuto", m_bAuto)
    fill("isDemon", m_bDemon)
    fill("areCoinsVerified", m_bVerifiedCoins)
    fill("ldmAvailable", m_bLDM)
    fill("is2P", m_b2P)

    fill_str("levelName", m_sLevelName)
    fill_str("levelDescription", m_sDescription)
    fill_str("uploadDate", m_sUploadDate)
    fill_str("updateDate", m_sUpdateDate)
    fill_str("username", m_sUsername);
    fill_str("actualGameVersion", m_uRelease->m_fActualVersion)

    return;
}

void Level::restore() {
    #define RS(t, str, val) val = levelJson[str].get<t>();
    
    RS(int, "levelID", m_nLevelID)
    RS(int, "version", m_nVersion)
    RS(int, "playerID", m_nPlayerID)
    RS(int, "downloads", m_nDownloads)
    RS(int, "musicID", m_nMusicID)
    RS(int, "likes", m_nLikes)
    RS(int, "length", m_nLength)
    RS(int, "difficulty_denominator", m_nDifficultyDenominator)
    RS(int, "difficulty_numerator", m_nDifficultyNumerator)
    RS(int, "fakeGameVersion", m_nGameVersion)
    RS(int, "dislikes", m_nDislikes)
    RS(int, "stars", m_nStars)
    RS(int, "featureScore", m_nFeatureScore)
    RS(int, "copiedFrom", m_nCopiedID)
    RS(int, "dailyNumber", m_nDailyNumber)
    RS(int, "coins", m_nCoins)
    RS(int, "starsReequested", m_nStarsRequested)
    RS(int, "isEpic", m_nEpic)
    RS(int, "demonDifficulty", m_nDemonDifficulty)
    RS(int, "editorTime", m_nEditorTime)
    RS(int, "editorTimeTotal", m_nEditorTimeTotal)
    RS(int, "accountID", m_nAccountID)

    RS(bool, "songID", m_nSongID)
    RS(bool, "isAuto", m_bAuto)
    RS(bool, "isDemon", m_bDemon)
    RS(bool, "areCoinsVerified", m_bVerifiedCoins)
    RS(bool, "ldmAvailable", m_bLDM)
    RS(bool, "is2P", m_b2P)

    RS(std::string, "levelName", *m_sLevelName)
    RS(std::string, "levelDescription", *m_sDescription)
    RS(std::string, "uploadDate", *m_sUploadDate)
    RS(std::string, "updateDate", *m_sUpdateDate)
    RS(std::string, "username", *m_sUsername)
    RS(std::string, "actualGameVersion", *m_uRelease->m_fActualVersion)

    std::string g = *m_sLevelPath + "/meta.json";

    std::ofstream file(g);
    file << levelJson.dump(4);

    return;
}

Level::~Level() {
    delete m_uRelease;
    // delete m_sLevelPath;
    // delete m_sUsername;
}