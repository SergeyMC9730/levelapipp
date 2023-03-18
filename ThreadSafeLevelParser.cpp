#include "ThreadSafeLevelParser.h"
#include "StringSplit.h"
#include "lapi_database.h"
#include "gmd2pp/level-converter/base64.h"

using namespace LevelAPI::Backend;

#define PKSTRING 0
#define PKINT 1
#define PKBOOL 2
#define PKBASE64 3

#define PARSE_KEY_PKSTRING(keyN, member) \
    case keyN: {\
        member = new std::string(info[i]); \
        break; \
    }
#define PARSE_KEY_PKINT(keyN, member) \
    case keyN: {\
        member = std::stoi(info[i]); \
        break; \
    }
#define PARSE_KEY_PKBOOL(keyN, member) \
    case keyN: {\
        member = (bool)(std::stoi(info[i])); \
        break; \
    }
#define PARSE_KEY_PKBASE64(keyN, member) \
    case keyN: {\
        member = new std::string((char *)base64_decode(info[i]).data()); \
        break; \
    }
#define PARSE_KEY(keyN, member, vType) PARSE_KEY_##vType(keyN, member)

LevelAPI::DatabaseController::Level *LevelParser::parseFromResponse(const char *response) {
    DatabaseController::Level *level = new DatabaseController::Level();

    auto info = splitString(response, ':');

    bool doKey = true;
    int currentKey;

    int i = 0;
    while(i < info.size()) {
        currentKey = std::stoi(info[i]);
        i++;
        switch(currentKey) {
            PARSE_KEY(1, level->m_nLevelID, PKINT);
            PARSE_KEY(5, level->m_nVersion, PKINT);
            PARSE_KEY(6, level->m_nPlayerID, PKINT);
            PARSE_KEY(8, level->m_nDifficultyDenominator, PKINT);
            PARSE_KEY(9, level->m_nDifficultyNumerator, PKINT);
            PARSE_KEY(10, level->m_nDownloads, PKINT);
            PARSE_KEY(11, level->m_nSetCompletes, PKINT);
            PARSE_KEY(12, level->m_nMusicID, PKINT);
            PARSE_KEY(13, level->m_nGameVersion, PKINT);
            PARSE_KEY(14, level->m_nLikes, PKINT);
            PARSE_KEY(15, level->m_nLength, PKINT);
            PARSE_KEY(16, level->m_nDislikes, PKINT);
            PARSE_KEY(18, level->m_nStars, PKINT);
            PARSE_KEY(19, level->m_nFeatureScore, PKINT);
            PARSE_KEY(30, level->m_nSongID, PKINT);
            PARSE_KEY(37, level->m_nCoins, PKINT);
            PARSE_KEY(39, level->m_nStarsRequested, PKINT);
            PARSE_KEY(41, level->m_nDailyNumber, PKINT);
            PARSE_KEY(42, level->m_nEpic, PKINT);
            PARSE_KEY(43, level->m_nDemonDifficulty, PKINT);
            PARSE_KEY(45, level->m_nObjects, PKINT);
            PARSE_KEY(46, level->m_nEditorTime, PKINT);
            PARSE_KEY(47, level->m_nEditorTimeTotal, PKINT);

            PARSE_KEY(17, level->m_bDemon, PKBOOL);
            PARSE_KEY(25, level->m_bAuto, PKBOOL);
            PARSE_KEY(31, level->m_b2P, PKBOOL);
            PARSE_KEY(38, level->m_bVerifiedCoins, PKBOOL);
            PARSE_KEY(40, level->m_bLDM, PKBOOL);
            PARSE_KEY(44, level->m_bGauntlet, PKBOOL);

            PARSE_KEY(2, level->m_sLevelName, PKSTRING);
            PARSE_KEY(4, level->m_sLevelString, PKSTRING);
            PARSE_KEY(26, level->m_sRecordString, PKSTRING);
            PARSE_KEY(27, level->m_sXORPassword, PKSTRING);
            PARSE_KEY(28, level->m_sUploadDate, PKSTRING);
            PARSE_KEY(29, level->m_sUpdateDate, PKSTRING);
            PARSE_KEY(36, level->m_sExtraString, PKSTRING);
            PARSE_KEY(48, level->m_sSettings, PKSTRING);

            PARSE_KEY(3, level->m_sDescription, PKBASE64);
        }
        i++;
    }

    level->m_uRelease->m_fActualVersion = new std::string(DatabaseController::LevelRelease::determineFromID(level->m_nLevelID));
    level->m_uRelease->m_nGameVersion = level->m_nGameVersion;

    return level;
}