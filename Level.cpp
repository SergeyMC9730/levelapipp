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

#include "Level.h"
#include "SQLiteManager.h"
#include "lapi_database.h"
#include "json/single_include/nlohmann/json.hpp"
#include "gmd2pp/gmd2.h"
#ifdef _DPP_ENBLED_
#include "message.h"
#endif
#include <filesystem>
#include <fstream>
#include "Time.h"
#include <random>
#include "Translation.h"
#include <string>
#include <sys/stat.h>
#include <vector>
#ifdef _HTTPSERVER_HPP_INSIDE_
#include "HttpController.h"
#endif
#include "LevelRelease.h"

using namespace LevelAPI::DatabaseController;
using namespace LevelAPI::Frontend;

Level::Level(std::string linkedNode) {
    m_uRelease = new LevelRelease();

    m_bHasLevelString = false;
    m_uRelease->m_nGameVersion = 0;

    Frontend::Time *t = Frontend::Time::create();
    m_sCreatedTimestamp = t->getAsString();
    m_nAppereanceTimestamp = t->unixTime;
    
    m_sLinkedNode = linkedNode;

    delete t;
    t = nullptr;

    setupJSON();
}

void Level::setupJSON() {
    _jsonObject = nlohmann::json();
}

Time *Level::getTimeLegacy() {
    if (m_sCreatedTimestamp.empty()) return nullptr;

    Time *t = Time::create();

    t->fromTimeString(m_sCreatedTimestamp);

    return t;
}

void Level::save(bool onlyLevelString) {
    std::string g2 = m_sLevelPath + "/data.gmd2";

    auto lambda_save = [&]() {
        if (m_bHasLevelString) {
            auto gmd2file = new GMD2();
            gmd2file->setFileName(g2);
            gmd2file->setDebug(false);
            gmd2file->setLevel(this);
            gmd2file->generate();

            delete gmd2file;
            gmd2file = nullptr;
        }
    };

    if (onlyLevelString) {
        lambda_save();
        
        return;
    }

    generateDifficultyImage("resources");

    nlohmann::json _sfx = m_vSFXList;
    nlohmann::json _songs = m_vMusicList;

    SQLiteRow rw = {
        {"version", m_nVersion},
        {"playerID", m_nPlayerID},
        {"downloads", m_nDownloads},
        {"musicID", m_nMusicID},
        {"likes", m_nLikes},
        {"length", m_nLength},
        {"difficulty_denominator", m_nDifficultyDenominator},
        {"difficulty_numenator", m_nDifficultyNumerator},
        {"fakeGameVersion", m_nGameVersion},
        {"dislikes", m_nDislikes},
        {"stars", m_nStars},
        {"featureScore", m_nFeatureScore},
        {"copiedFrom", m_nCopiedID},
        {"dailyNumber", m_nDailyNumber},
        {"coins", m_nCoins},
        {"starsRequested", m_nStarsRequested},
        {"isEpic", m_nEpic},
        {"demonDifficulty", m_nDemonDifficulty},
        {"editorTime", m_nEditorTime},
        {"editorTimeTotal", m_nEditorTimeTotal},
        {"accountID", m_nAccountID},
        {"songID", m_nSongID},
        {"objects", m_nObjects},
        {"moons", m_nMoons},
        {"isAuto", m_bAuto},
        {"isDemon", m_bDemon},
        {"areCoinsVerified", m_bVerifiedCoins},
        {"ldmAvailable", m_bLDM},
        {"is2P", m_b2P},
        {"levelName", m_sLevelName},
        {"levelDescription", m_sDescription},
        {"uploadDate", m_sUploadDate},
        {"updateDate", m_sUpdateDate},
        {"username", m_sUsername},
        {"actualGameVersion", m_uRelease->m_fActualVersion},
        {"databaseAppereanceDate", (uint64_t)m_nAppereanceTimestamp},
        {"levelID", m_nLevelID},
        {"songIDs", _songs.dump()},
        {"sfxIDs", _sfx.dump()}
    };

    lambda_save();

    std::string g = m_sLevelPath + "/meta.json";

    if (!onlyLevelString) {
        auto node = DatabaseController::database->getNode(m_sLinkedNode);

        node->_sqliteObject->pushRow(rw, "levels");
    }

    return;
}

std::string Level::getDownloadLinks(bool embed) {
#ifdef _HTTPSERVER_HPP_INSIDE_
    std::string url = HttpController::getURL();

    bool hasGMD2 = m_bHasLevelString;

    std::string result = fmt::format("[{}]({}/?id={}&node={})",
        Translation::getByKey("lapi.level.embed.field.info.value.metadata"),
        url,
        this->m_nLevelID,
        m_sLinkedNode
    );

    if (hasGMD2) {
        result += fmt::format(" | [{}]({}/api/v1/level/download?id={}&node={}&type=1)",
            Translation::getByKey("lapi.level.embed.gmd2.download"),
            url,
            this->m_nLevelID,
            m_sLinkedNode
        );
    }

    return result;
#else
    return "";
#endif
}

void Level::recover() {
#define RS(t, str, val) if (_jsonObject.contains(str)) {if(!_jsonObject[str].is_null()) val = _jsonObject[str].get<t>();}

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
    RS(int, "objects", m_nObjects)
    RS(int, "moons", m_nMoons);

    RS(uint64_t, "appereanceTimestamp", m_nAppereanceTimestamp);

    RS(bool, "isAuto", m_bAuto)
    RS(bool, "isDemon", m_bDemon)
    RS(bool, "areCoinsVerified", m_bVerifiedCoins)
    RS(bool, "ldmAvailable", m_bLDM)
    RS(bool, "is2P", m_b2P)

    RS(std::string, "levelName", m_sLevelName)
    RS(std::string, "levelDescription", m_sDescription)
    RS(std::string, "uploadDate", m_sUploadDate)
    RS(std::string, "updateDate", m_sUpdateDate)
    RS(std::string, "username", m_sUsername)
    RS(std::string, "actualGameVersion", m_uRelease->m_fActualVersion)
    RS(std::string, "publicationDate", m_sCreatedTimestamp);

    if (m_nAppereanceTimestamp != 0) {
        Time *t = new Time(m_nAppereanceTimestamp);

        m_sCreatedTimestamp = t->getAsString();

        delete t;
        t = nullptr;
    } else {
        auto time_legacy = getTimeLegacy();

        if (time_legacy) {
            m_nAppereanceTimestamp = time_legacy->unixTime;

            delete time_legacy;
            time_legacy = nullptr;
        }
    }

    auto node = DatabaseController::database->getNode(m_sLinkedNode);
    std::string p1 = fmt::format("database/nodes/{}/levels/{}", m_sLinkedNode, node->getLevelPathRepresentation(m_nLevelID));

    if (std::filesystem::exists(p1)) {
        m_bHasLevelString = true;
    }

    RS(std::vector<int>, "songIDs", m_vMusicList);
    RS(std::vector<int>, "sfxIDs", m_vSFXList);

#undef RS

    return;
}

Level::~Level() {
    delete m_uRelease;
    m_uRelease = nullptr;
}

#include <raylib.h>

std::string Level::generateDifficultyImage(std::string folder_prefix) {
    std::vector<Image> layers = {};

    int stars;
    std::vector<bool> parameters;

    std::string path;
    std::string file = "rating_";
    std::string diffimage;

    parameters.push_back(m_nFeatureScore > 0);
    parameters.push_back(m_nEpic);
    if(parameters[1]) parameters[0] = false;

    switch(m_nStars == 0 ? m_nStarsRequested : m_nStars) {
        case 1:
        case 2: {
            diffimage = "easy";
            break;
        }
        case 3: {
            diffimage = "normal";
            break;
        }
        case 4:
        case 5: {
            diffimage = "hard";
            break;
        }
        case 6:
        case 7: {
            diffimage = "harder";
            break;
        }
        case 8:
        case 9: {
            diffimage = "insane";
            break;
        }
        case 10: {
            diffimage = "demon";
            break;
        }
        default: {
            diffimage = "na";
            break;
        }
    }

    stars = m_nStars;

    if(m_bAuto) diffimage = "auto";
    if(m_bDemon) diffimage = "demon";

    file += diffimage + std::to_string(stars) + "_";
    int i = 0;
    while(i < parameters.size()) {
        file += (parameters[i]) ? "y" : "n";
        i++;
    }
    file += "_new.png";

    path = folder_prefix + "/" + file;

    if(std::filesystem::exists(path)) {
        return file;
    }

    if(parameters[0]) {
        // mats.push_back(cv::imread(folder_prefix + "/feature.png", cv::IMREAD_UNCHANGED));  
        std::string p = folder_prefix + "/feature.png";
        layers.push_back(LoadImage(p.c_str()));
    };
    if(parameters[1]) {
        std::string p = folder_prefix + "/epic.png";
        layers.push_back(LoadImage(p.c_str()));
    };
    if(stars <= 10 && stars >= 0) {
        std::string p = folder_prefix + "/star" + std::to_string(stars) + ".png";
        layers.push_back(LoadImage(p.c_str()));
    }

    std::string p = folder_prefix + "/" + diffimage + ".png";
    layers.push_back(LoadImage(p.c_str()));

    Image final = LoadImage((folder_prefix + "/empty.png").c_str());

    for (Image img : layers) {
        Rectangle r1 = {
            .width = (float)img.width,
            .height = (float)img.height
        };
        Rectangle r2 = {
            .width = (float)final.width,
            .height = (float)final.height
        };

        ImageDraw(&final, img, r1, r2, WHITE);

        UnloadImage(img);
    }

    ExportImage(final, path.c_str());

    UnloadImage(final);

    return path;
}

#ifdef _DPP_ENABLED_
dpp::embed Level::getAsEmbed(LevelAppearanceEvent e) {
    std::vector<std::string> eventtable = {
        "lapi.level.embed.description",
        "lapi.level.register.embed.description",
        "lapi.level.rate.embed.description",
        "lapi.level.info.embed.description",
    };

    std::vector<std::string> titletable = {
        "lapi.level.embed.title",
        "lapi.level.embed.title",
        "lapi.level.embed.rate.title",
        "lapi.level.embed.info.title",
    };

#ifdef _HTTPSERVER_HPP_INSIDE_
    std::string url = HttpController::getURL();
#else
    std::string url = "https://www.google.com";
#endif

    std::string img_path = generateDifficultyImage("resources");

    std::string gv = std::to_string((float)m_nGameVersion / 10.f);
    gv.erase(gv.find_last_not_of('0') + 1, std::string::npos);
    gv.erase(gv.find_last_not_of('.') + 1, std::string::npos);

    std::string msg = Translation::getByKey(eventtable[e], gv, m_sCreatedTimestamp);

    std::string thumbnail = url + "/api/v1/res/request/" + img_path;    

    std::random_device rd;
    std::uniform_int_distribution<int> uid(0, m_nLevelID);
    
    dpp::embed embed = dpp::embed().
        set_color(uid(rd)).
        set_title(Translation::getByKey(titletable[e])).
        set_description(msg).
        add_field(
            Translation::getByKey("lapi.level.embed.field.id"),
            "**`" + std::to_string(this->m_nLevelID) + "`**",
            true
        ).
        add_field(
            Translation::getByKey("lapi.level.embed.field.name"),
            "**" + this->m_sLevelName + "**",
            true
        ).
        add_field(
            Translation::getByKey("lapi.level.embed.field.author"),
            "**" + this->m_sUsername + "**",
            true
        ).
        set_thumbnail(thumbnail).
        set_footer(dpp::embed_footer().set_text("LevelAPI  •  " + m_sLinkedNode)).
        set_timestamp(this->m_nAppereanceTimestamp
    );

    if (e == LevelAppearanceEvent::E_INFORMATION) {
        embed.add_field(
            Translation::getByKey("lapi.level.embed.field.downloads"),
            "**`" + std::to_string(this->m_nDownloads) + "`**",
            true
        );

        embed.add_field(
            Translation::getByKey("lapi.level.embed.field.likes"),
            "**`" + std::to_string(this->m_nLikes) + "`**",
            true
        );

        embed.add_field(
            Translation::getByKey("lapi.level.embed.field.gv"),
            "**`" + this->m_uRelease->m_fActualVersion + "`**",
            true
        );
    }

#ifdef _HTTPSERVER_HPP_INSIDE_
    embed.add_field(
        Translation::getByKey("lapi.level.embed.field.info"),
        getDownloadLinks(true),
        true
    );
#endif

    return embed;
}
#endif
