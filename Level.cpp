#include "lapi_database.h"
#include "Tools.h"
#include "json/single_include/nlohmann/json.hpp"
#include "gmd2pp/gmd2.h"
#include <fstream>

using namespace LevelAPI::DatabaseController;

Level::Level() {
    m_uRelease = new LevelRelease();
    m_uRelease->m_fActualVersion = new std::string(m_uRelease->determineFromID(m_nLevelID));

    m_sLevelPath = new std::string(".");
    m_sUsername = new std::string("");
    m_sLevelName = new std::string("");
    m_sDescription = new std::string("");
    m_sUploadDate = new std::string("");
    m_sUpdateDate = new std::string("");
    m_sLevelString = new std::string("");
    m_sSettings = new std::string("");
    m_sExtraString = new std::string("");
    m_sXORPassword = new std::string("");
    m_sRecordString = new std::string("");

    m_nDislikes = 0;

    setupJSON();
}

void Level::setupJSON() {
    levelJson = nlohmann::json();
}

void Level::save() {
    #define fill(str, val) levelJson[str] = val;
    #define fill_str(str, val) fill(str, *val);

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
    fill("starsRequested", m_nStarsRequested)
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

    std::string g = *m_sLevelPath + "/meta.json";
    std::string g2 = *m_sLevelPath + "/data.gmd2";

    std::ofstream file(g);
    file << levelJson.dump(4);

    auto gmd2file = new GMD2();
    gmd2file->setFileName(g2);
    gmd2file->setDebug(false);
    gmd2file->setLevel(this);
    gmd2file->generate();

    delete gmd2file;
    file.close();

    return;
}

void Level::restore() {
    #define RS(t, str, val) val = levelJson[str].get<t>();
    #define RSS(t, str, val) delete val; val = new t (levelJson[str].get<t>());
    
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
    RS(int, "starsRequested", m_nStarsRequested)
    RS(int, "isEpic", m_nEpic)
    RS(int, "demonDifficulty", m_nDemonDifficulty)
    RS(int, "editorTime", m_nEditorTime)
    RS(int, "editorTimeTotal", m_nEditorTimeTotal)
    RS(int, "accountID", m_nAccountID)
    RS(int, "songID", m_nSongID)

    RS(bool, "isAuto", m_bAuto)
    RS(bool, "isDemon", m_bDemon)
    RS(bool, "areCoinsVerified", m_bVerifiedCoins)
    RS(bool, "ldmAvailable", m_bLDM)
    RS(bool, "is2P", m_b2P)

    RSS(std::string, "levelName", m_sLevelName)
    RSS(std::string, "levelDescription", m_sDescription)
    RSS(std::string, "uploadDate", m_sUploadDate)
    RSS(std::string, "updateDate", m_sUpdateDate)
    RSS(std::string, "username", m_sUsername)
    RSS(std::string, "actualGameVersion", m_uRelease->m_fActualVersion)

    return;
}

Level::~Level() {
    delete m_uRelease;
    delete m_sLevelPath;
    delete m_sUsername;
    delete m_sLevelName;
    delete m_sDescription;
    delete m_sUploadDate;
    delete m_sUpdateDate;
}