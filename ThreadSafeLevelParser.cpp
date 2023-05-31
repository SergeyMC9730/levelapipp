#include <iostream>
#include <stdexcept>
#include <string_view>

#include "RobTopStringContainer.hpp"
#include "ThreadSafeLevelParser.h"
#include "StringSplit.h"
#include "lapi_database.h"
#include "gmd2pp/base64.h"
#include "Translation.h"

using namespace LevelAPI::Backend;
using namespace LevelAPI::Frontend;

#define GKSTRING 0
#define GKINT 1
#define GKBOOL 2
#define GKBASE64 3

#define GET_KEY_GKSTRING(keyN, member) \
    if (container->variableExists(keyN)) member = std::string(std::get<std::string_view>(container->getVariable(keyN)));
#define GET_KEY_GKINT(keyN, member) \
    if (container->variableExists(keyN)) member = std::get<int>(container->getVariable(keyN));
#define GET_KEY_GKBOOL(keyN, member) \
    if (container->variableExists(keyN)) member = std::get<bool>(container->getVariable(keyN));
#define GET_KEY_GKBASE64(keyN, member) GET_KEY_GKSTRING(keyN, member);
#define GET_KEY(keyN, member, vType) GET_KEY_##vType(keyN, member)

LevelAPI::DatabaseController::Level *LevelParser::parseFromResponse(const char *response) { 
    return LevelParser::parseFromResponse(response, true);
}

LevelAPI::DatabaseController::Level *LevelParser::parseFromResponse(const char *response, bool decrypt_description) {
    //printf("response: %s\n", response);
    DatabaseController::Level *level = new DatabaseController::Level();
    RobTopStringContainer *container = new RobTopStringContainer(response);

    container->setParserForVariable(
        {
            1,  5,  6,   8,
            9,  10, 11, 12,
            13, 14, 15, 16,
            18, 19, 30, 35,
            37, 39, 41, 42,
            43, 45, 46,  47
        },
        [&](std::string_view input, int id) {
            try {
                return std::stoi(std::string(input));
            } catch (std::invalid_argument &e) {
                return 0;
            }
        }
    );
    container->setParserForVariable(
        {
            17, 25,
            31, 38,
            40,  44
        },
        [&](std::string_view input, int id) {
            int v = 0;
            try {
                v = std::stoi(std::string(input));
            } catch (std::invalid_argument &e) {
                return false;
            }

            return v >= 1;
        }
    );
    container->setParserForVariable(
        {
            2,  4,  26, 27,
            28, 29, 36, 48
        },
        [&](std::string_view input, int id) {
            return input;
        }
    );
    container->setParserForVariable(
        3,
        [&](std::string_view input, int id, int carg) {
            auto input2 = std::string(input);

            try {
                if(true) {
                    if(input.compare("")) {
                        std::string_view member = (char *)(input2).data();
                        return member;
                    }
                } else {
                    std::string_view member = input;
                    return member;
                }
            }
            catch (std::logic_error &e) {
                std::cout << Translation::getByKey("lapi.tslp.logicerror") << std::endl;
                std::string_view member = "ERROR";
                return member;
            }

            return input;
        },
        static_cast<int>(decrypt_description)
    );

    level->m_sRawData = response;

    container->parse();

    level->m_nLevelID = std::get<int>(container->getVariable(1));

    GET_KEY(1, level->m_nLevelID, GKINT);
    GET_KEY(5, level->m_nVersion, GKINT);
    GET_KEY(6, level->m_nPlayerID, GKINT);
    GET_KEY(8, level->m_nDifficultyDenominator, GKINT);
    GET_KEY(9, level->m_nDifficultyNumerator, GKINT);
    GET_KEY(10, level->m_nDownloads, GKINT);
    GET_KEY(11, level->m_nSetCompletes, GKINT);
    GET_KEY(12, level->m_nMusicID, GKINT);
    GET_KEY(13, level->m_nGameVersion, GKINT);
    GET_KEY(14, level->m_nLikes, GKINT);
    GET_KEY(15, level->m_nLength, GKINT);
    GET_KEY(16, level->m_nDislikes, GKINT);
    GET_KEY(18, level->m_nStars, GKINT);
    GET_KEY(19, level->m_nFeatureScore, GKINT);
    GET_KEY(30, level->m_nCopiedID, GKINT);
    GET_KEY(35, level->m_nSongID, GKINT);
    GET_KEY(37, level->m_nCoins, GKINT);
    GET_KEY(39, level->m_nStarsRequested, GKINT);
    GET_KEY(41, level->m_nDailyNumber, GKINT);
    GET_KEY(42, level->m_nEpic, GKINT);
    GET_KEY(43, level->m_nDemonDifficulty, GKINT);
    GET_KEY(45, level->m_nObjects, GKINT);
    GET_KEY(46, level->m_nEditorTime, GKINT);
    GET_KEY(47, level->m_nEditorTimeTotal, GKINT);

    GET_KEY(17, level->m_bDemon, GKBOOL);
    GET_KEY(25, level->m_bAuto, GKBOOL);
    GET_KEY(31, level->m_b2P, GKBOOL);
    GET_KEY(38, level->m_bVerifiedCoins, GKBOOL);
    GET_KEY(40, level->m_bLDM, GKBOOL);
    GET_KEY(44, level->m_bGauntlet, GKBOOL);

    GET_KEY(2, level->m_sLevelName, GKSTRING);
    GET_KEY(4, level->m_sLevelString, GKSTRING);
    GET_KEY(26, level->m_sRecordString, GKSTRING);
    GET_KEY(27, level->m_sXORPassword, GKSTRING);
    GET_KEY(28, level->m_sUploadDate, GKSTRING);
    GET_KEY(29, level->m_sUpdateDate, GKSTRING);
    GET_KEY(36, level->m_sExtraString, GKSTRING);
    GET_KEY(48, level->m_sSettings, GKSTRING);

    GET_KEY(3, level->m_sDescription, GKBASE64);

    return level;
}
