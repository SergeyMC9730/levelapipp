/**
 *  LevelAPI - Geometry Dash level cacher with search functionality and more.
    Copyright (C) 2023  Sergei Baigerov

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <https://www.gnu.org/licenses/>.
*/

#include <iostream>
#include <stdexcept>

#include "RobTopStringContainer.hpp"
#include "ThreadSafeLevelParser.h"
#include "StringSplit.h"
#include "gmd2pp/cpp-base64/base64.h"
#include "Translation.h"

#include "Level.h"

using namespace LevelAPI::Backend;
using namespace LevelAPI::Frontend;

LevelAPI::DatabaseController::Level *LevelParser::parseFromResponse(std::string response, bool decrypt_description) {
    //printf("response: %s\n", response);
    DatabaseController::Level *level = new DatabaseController::Level("");
    RobTopStringContainer *container = new RobTopStringContainer(response);

    container->setParserForVariable(
        {
            1,  5,  6,   8,
            9,  10, 11, 12,
            13, 14, 15, 16,
            18, 19, 30, 35,
            37, 39, 41, 42,
            43, 45, 46,  47,
            54, 57
        },
        [&](std::string input, int id) {
            try {
                return std::stoi(input);
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
        [&](std::string input, int id) {
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
        [&](std::string input, int id) {
            return input;
        }
    );
    container->setParserForVariable(
        3,
        [&](std::string input, int id, int carg) {
            auto input2 = std::string(input);

            try {
                if(carg) {
                    if(input.compare("")) {
                        std::string decoded = "";

                        try {
                            decoded = base64_decode(input);
                        } catch (std::runtime_error err) {
                            std::cout << err.what() << std::endl;
                        }
                        
                        return decoded;
                    }
                } else {
                    return input;
                }
            }
            catch (std::logic_error &e) {
                std::cout << Translation::getByKey("lapi.tslp.logicerror") << std::endl;
                std::string err = "ERROR";
                return err;
            }

            return input;
        },
        static_cast<int>(decrypt_description)
    );
    container->setParserForVariable(
        {
            52, 53
        },
        [&](std::string input, int id) {
            std::vector<int> vec;

            fmt::print("KEY {}: reading {}\n", id, input);

            if (input.empty()) return vec;
            if (input[0] == '#') return vec;

            std::vector<std::string> vals = splitString(input.c_str(), ',');

            for (std::string _val : vals) {
                try {
                    vec.push_back(std::stoi(_val));
                } catch (std::invalid_argument e) {
                    std::cout << Translation::getByKey("lapi.tslp.invalidargument") << std::endl;
                    continue;
                }
            }

            return vec;
        }
    );

    container->setParserForVariable(
        54,
        [&](std::string input, int id) {
            return input == "1";
        }
    );
    container->setParserForVariable(
        57,
        [&](std::string input, int id) {
            return input == "1";
        }
    );

    level->m_sRawData = response;

    container->parse();

    GET_KEY(container, 1, level->m_nLevelID, GKINT);
    GET_KEY(container, 5, level->m_nVersion, GKINT);
    GET_KEY(container, 6, level->m_nPlayerID, GKINT);
    GET_KEY(container, 8, level->m_nDifficultyDenominator, GKINT);
    GET_KEY(container, 9, level->m_nDifficultyNumerator, GKINT);
    GET_KEY(container, 10, level->m_nDownloads, GKINT);
    GET_KEY(container, 11, level->m_nSetCompletes, GKINT);
    GET_KEY(container, 12, level->m_nMusicID, GKINT);
    GET_KEY(container, 13, level->m_nGameVersion, GKINT);
    GET_KEY(container, 14, level->m_nLikes, GKINT);
    GET_KEY(container, 15, level->m_nLength, GKINT);
    GET_KEY(container, 16, level->m_nDislikes, GKINT);
    GET_KEY(container, 18, level->m_nStars, GKINT);
    GET_KEY(container, 19, level->m_nFeatureScore, GKINT);
    GET_KEY(container, 30, level->m_nCopiedID, GKINT);
    GET_KEY(container, 35, level->m_nSongID, GKINT);
    GET_KEY(container, 37, level->m_nCoins, GKINT);
    GET_KEY(container, 39, level->m_nStarsRequested, GKINT);
    GET_KEY(container, 41, level->m_nDailyNumber, GKINT);
    GET_KEY(container, 42, level->m_nEpic, GKINT);
    GET_KEY(container, 43, level->m_nDemonDifficulty, GKINT);
    GET_KEY(container, 45, level->m_nObjects, GKINT);
    GET_KEY(container, 46, level->m_nEditorTime, GKINT);
    GET_KEY(container, 47, level->m_nEditorTimeTotal, GKINT);

    GET_KEY(container, 17, level->m_bDemon, GKBOOL);
    GET_KEY(container, 25, level->m_bAuto, GKBOOL);
    GET_KEY(container, 31, level->m_b2P, GKBOOL);
    GET_KEY(container, 38, level->m_bVerifiedCoins, GKBOOL);
    GET_KEY(container, 40, level->m_bLDM, GKBOOL);
    GET_KEY(container, 44, level->m_bGauntlet, GKBOOL);

    GET_KEY(container, 2, level->m_sLevelName, GKSTRING);
    GET_KEY(container, 4, level->m_sLevelString, GKSTRING);
    GET_KEY(container, 26, level->m_sRecordString, GKSTRING);
    GET_KEY(container, 27, level->m_sXORPassword, GKSTRING);
    GET_KEY(container, 28, level->m_sUploadDate, GKSTRING);
    GET_KEY(container, 29, level->m_sUpdateDate, GKSTRING);
    GET_KEY(container, 36, level->m_sExtraString, GKSTRING);
    GET_KEY(container, 48, level->m_sSettings, GKSTRING);

    GET_KEY(container, 3, level->m_sDescription, GKBASE64);

    GET_KEY(container, 52, level->m_vMusicList, GKVINT);
    GET_KEY(container, 53, level->m_vSFXList, GKVINT);

    delete container;
    container = nullptr;

    if (level->m_nEpic == 2) {
        level->m_bLegendary = true;
    } else if (level->m_nEpic >= 3) {
        level->m_bMythic = true;
    }

    if (level->m_nLength == 5) {
        level->m_nMoons = level->m_nStars;
        level->m_nStars = 0;
    }

    return level;
}
