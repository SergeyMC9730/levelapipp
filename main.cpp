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
#include <httpserver.hpp>
#include <iterator>
#include <dirent.h>
#include <sys/statvfs.h>
#include <thread>
#include <vector>

#include "GDServer.h"
#include "Level.h"

#include "TestingBoomlings22.h"
// #include "connection.h"
#include "requests/requests.h"

#include "gmd2pp/gmd2.h"

#include "lapi_database.h"
#include "lapi_version.h"

#include "HttpController.h"

#include "GDServer_BoomlingsLike21.h"

#include "UUID.h"

#include <ctime>

// #include "sqlite3/connection.h"
// #include "sqlite3/connection_config.h"

#include "SQLiteManager.h"

#include "tests.h"

#include "Translation.h"

#include <chrono>

#include <iostream>

// #include <sqlpp11/sqlpp11.h>
// #include <sqlpp11/sqlite3/sqlite3.h>

using namespace LevelAPI;
using namespace LevelAPI::Frontend::Translation;

std::vector<std::string> get_tests() {
    return {
        "boomlings2.2",
        "basement",
        "robtop-parser"
    };
}

void print_tests() {
    auto tests = get_tests();

    int i = 0;

    std::cout << "Available tests:" << std::endl;
    
    while(i < tests.size()) {
        auto test = tests[i];

        std::cout << " - " << test << std::endl;

        i++;
    }

    return;
}

bool is_test_valid(std::string test) {
    auto tests = get_tests();

    int i = 0;
    while(i < tests.size()) {
        if (tests[i] == test) return true;
        i++;
    }

    return false;
}

void route_test(std::string test) {
    if (!is_test_valid(test)) return;

    if (test == "boomlings2.2") {
        LevelAPI::Tests::test_boomlings_ver22(); return;
    }
    if (test == "basement") {
        LevelAPI::Tests::testBasementFeatures(); return;
    }
    if (test == "robtop-parser") {
        LevelAPI::Tests::testRobtopParser(); return;
    }

    return;
}

#include "GenericTools.hpp"

int testcallback(void *sql, int columns, char **array1, char **array2) {
    int i = 0;

    std::cout << "Columns: " << columns << std::endl;

    // std::cout << "ARRAY 1:" << std::endl;
    // while (array1[i] != NULL) {
    //     std::cout << array1[i] << std::endl; 
    //     i++;
    // }

    // i = 0;

    // std::cout << "ARRAY 2:" << std::endl;
    // while (array2[i] != NULL) {
    //     std::cout << array2[i] << std::endl; 
    //     i++;
    // }

    return 0;
}

#include "GenericTools.hpp"
#include <algorithm>

std::string makeStrLowercase(std::string _str) {
    if (_str.empty()) return _str;

    std::string str = _str;

    std::transform(str.begin(), str.end(), str.begin(),
    [](unsigned char c){ return std::tolower(c); });

    return str;
}

bool setupAskQuestion(std::string q, bool default_yes) {
    std::vector<std::string> varYes = {"y", "y", "yes", "Y", "Yes", "yEs", "yeS", "YES", "YeS"};
    std::vector<std::string> varNo = {"n", "n", "no", "N", "No", "nO", "NO"};

    if (default_yes) {
        varYes[0][0] = std::toupper(varYes[0][0]);
        varYes.push_back("");
    } else {
        varNo[0][0] = std::toupper(varNo[0][0]);
        varNo.push_back("");
    }

    fmt::print("{} {}? ({}/{}): ", getByKey("lapi.main.question"), q, varYes[0], varNo[0]);

    std::string res;
    bool ret = false;
    bool found_variant = false;

    std::cin >> res;

    for (std::string var : varYes) {
        if (res == var) {
            ret = true;
            found_variant = true;

            break;
        }
    }

    if (!found_variant) {
        for (std::string var : varYes) {
            if (res == var) {
                ret = false;
                found_variant = true;

                break;
            }
        }
    }

    if (!found_variant) return false;

    return ret;
}

#include <termios.h>

// https://stackoverflow.com/questions/13694170/how-do-i-hide-user-input-with-cin-in-c
void HideStdinKeystrokes()
{
    termios tty;

    tcgetattr(STDIN_FILENO, &tty);

    /* we want to disable echo */
    tty.c_lflag &= ~ECHO;

    tcsetattr(STDIN_FILENO, TCSANOW, &tty);
}

// https://stackoverflow.com/questions/13694170/how-do-i-hide-user-input-with-cin-in-c
void ShowStdinKeystrokes()
{
    termios tty;

    tcgetattr(STDIN_FILENO, &tty);

    /* we want to reenable echo */
    tty.c_lflag |= ECHO;

    tcsetattr(STDIN_FILENO, TCSANOW, &tty);
}

nlohmann::json setupInfoMakeNode() {
    nlohmann::json ret;

    std::cout << getByKey("lapi.main.node.db");

    nlohmann::json database;

    {
        std::string endpoint;
        std::cout << getByKey("lapi.main.node.endpoint");

        std::cin >> endpoint;

        bool valid_feature_set = false;
        auto feature_set_list = LevelAPI::DatabaseController::NodeDatabase::getFeatureSetsList();
        int feature_set = feature_set_list[0];

        while (!valid_feature_set) {
            fmt::print("{} {}{}: {}.\n  {} ",
                getByKey("lapi.main.node.fs.1"),
                feature_set_list.size(),
                getByKey("lapi.main.node.fs.2"),
                GenericTools::convertFromVector(feature_set_list),
                getByKey("lapi.main.node.fs.3")
            );

            int value;
            std::cin >> value;

            for (int fs : feature_set_list) {
                if (value == fs) {
                    valid_feature_set = true;
                    break;
                }
            }

            if (!valid_feature_set) {
                std::cout << getByKey("lapi.main.node.fs.error");
            }
        }

        bool read_only = setupAskQuestion(getByKey("lapi.main.node.rl"), false);

        std::string mod;
        bool mod_valid = false;
        auto mods = LevelAPI::DatabaseController::NodeDatabase::getModificationsList();
        mods.push_back(getByKey("lapi.main.node.mod.none"));

        while (!mod_valid) {
            fmt::print("{}     ({})\n  {} ", 
                getByKey("lapi.main.node.mod.1"),
                GenericTools::convertFromVector(mods),
                getByKey("lapi.main.node.mod.2")
            );

            std::cin >> mod; 

            for (auto mod_obj : mods) {
                if (makeStrLowercase(mod) == mod_obj) {
                    mod_valid = true;
                    mod = makeStrLowercase(mod);

                    break;
                }
            }

            if (!mod_valid) {
                std::cout << getByKey("lapi.main.node.mod.error");
            }
        }

        database["endpoint"] = endpoint;
        database["featureSet"] = feature_set;
        database["readOnly"] = read_only;
        database["modifications"] = mod;
    }

    ret["database"] = database;

    std::cout << getByKey("lapi.main.node.basic.in");

    {
        std::string internal_name;
        std::cin >> internal_name;

        ret["internalName"] = internal_name;
        ret["levelDataPath"] = "levels";
    }

    std::cout << getByKey("lapi.main.node.policy.error");

    nlohmann::json policy;

    {
        bool enable_lr = setupAskQuestion(getByKey("lapi.main.node.policy.1"), false);
        bool enable_recent_tab = setupAskQuestion(getByKey("lapi.main.node.policy.2"), true);
        bool enable_resolver = setupAskQuestion(getByKey("lapi.main.node.policy.3"), false);
        bool no_output = setupAskQuestion(getByKey("lapi.main.node.policy.4"), false);
        bool wait_rl = setupAskQuestion(getByKey("lapi.main.node.policy.5"), true);

        float qpi = 0.f;
        float ri = 0.f;

        std::cout << getByKey("lapi.main.node.policy.6");
        std::cin >> qpi;

        if (enable_resolver) {
            std::cout << getByKey("lapi.main.node.policy.7");
            std::cin >> ri;
        }

        policy["enableLinearResolver"] = enable_lr;
        policy["enableRecentTab"] = enable_recent_tab;
        policy["enableResolver"] = enable_resolver;
        policy["noOutput"] = no_output;
        policy["queueProcessingInterval"] = qpi;
        policy["resolverInterval"] = ri;
        policy["waitResolverRateLimit"] = wait_rl;      
    }

    ret["policy"] = policy;

    nlohmann::json queue;

    queue["commandQueue"] = nlohmann::json::array();
    queue["executeQueue"] = true;
    queue["runtimeState"] = 0;

    ret["queue"] = queue;

    std::cout << getByKey("lapi.main.node.success");

    return ret;
}

void setupInfo() {
    std::cout << "Setting up database\n";

    std::vector<std::string> languages_available = getLanguages();

    bool language_valid = false;
    std::string lang;

    while (!language_valid) {
        fmt::print("\n- Select language ({}) without the brackets: ", GenericTools::convertFromVector(languages_available));

        std::cin >> lang; 

        for (auto lang_obj : languages_available) {
            if (makeStrLowercase(lang) == lang_obj) {
                language_valid = true;
                lang = makeStrLowercase(lang);

                break;
            }
        }

        if (!language_valid) {
            std::cout << "* Error: Invalid language\n";
        }
    }

    nlohmann::json database;
    database["language"] = lang;

    translation_language = lang;

    bool make_node = setupAskQuestion(getByKey("lapi.main.node.new"), true);

    if (!make_node) {
        std::cout << getByKey("lapi.main.node.no");
    }

    nlohmann::json node_array = nlohmann::json::array();

    while (make_node) {
        node_array.push_back(setupInfoMakeNode());

        make_node = setupAskQuestion(getByKey("lapi.main.node.new"), true);
    }

    database["nodes"] = node_array;

    std::cout << "\n";

    bool with_discord = setupAskQuestion(getByKey("lapi.main.discord.toggle"), false);

    if (with_discord) {
        std::cout << getByKey("lapi.main.discord");

        std::string token;
        
        std::cout << getByKey("lapi.main.discord.token");
        HideStdinKeystrokes();
        std::cin >> token;

        ShowStdinKeystrokes();

        std::cout << getByKey("lapi.main.discord.warn");

        std::string registeredCID1 = "";
        std::string registeredCID2 = "";

        bool with_cid1 = setupAskQuestion(getByKey("lapi.main.discord.main"), true);
        if (with_cid1) {
            std::cout << getByKey("lapi.main.discord.chid");
            std::cin >> registeredCID1;
        }

        bool with_cid2 = setupAskQuestion(getByKey("lapi.main.discord.additional"), false);
        if (with_cid2) {
            std::cout << getByKey("lapi.main.discord.chid");
            std::cin >> registeredCID2;
        }

        database["botToken"] = token;
        database["registeredCID"] = registeredCID1;
        database["registeredCID2"] = registeredCID2;
    }
    
    std::cout << getByKey("lapi.main.db.save");

    LevelAPI::DatabaseController::make_directories();

    std::string p1 = "database/info.json";

    std::ofstream file(p1);
    file << database.dump(4);
}

int main(int argc, char *argv[]) {
    std::cout << "LevelAPI " << LEVELAPI_VERSION << "\n";
    std::cout << getByKey("lapi.main.alpha") << "\n\n";

    srand(time(nullptr));

    auto args = GenericTools::getArguments(argc, argv);

    if (args.size() != 0) {
        auto command = args[0];

        if (command == "test") {
            if (args.size() == 1) {
                print_tests();

                return 1;
            }

            auto subtest = args[1];

            if (!is_test_valid(subtest)) {
                print_tests();

                return 1;
            }

            route_test(subtest);

            return 0;
        }

        if (command == "setup") {
            setupInfo();

            return 0;
        }

        std::cout << "Help:\n - test  -> testing new LevelAPI features\n - setup -> setup LevelAPI database\n";

        return 1;
    }

    // manager->wipeTable("comments");

    // int i = 0;
    // while(i < 16) {
    //     manager->pushRow({
    //         {"message", "hi"},
    //         {"percentage", 100},
    //         {"userID", 1},
    //         {"levelID", 1}
    //     } , "comments");
    //     i++;
    // }

    // std::vector<std::map<std::string, std::string>>

    // std::cout << "Rows: " << manager->countTable("comments") << std::endl;

    // auto table = manager->getTableWithEquality("comments", "levelID", 1, {{"message", "hi"}, {"levelID", 1}});

    // i = 0;
    // while(i < table.size()) {
    //     auto map = table.at(i);
    //     for (auto [key, val] : map) {
    //         std::cout << key << ", " << val << std::endl;
    //     }
    //     i++;
    // }

    DatabaseController::setup();
    HttpController::setup();
    HttpController::parse();

    LevelAPI::Tests::testCurl();
    // LevelAPI::Tests::testGDParsers();

    webserver ws = create_webserver()
        .port(HttpController::getPort())
        .max_threads(HttpController::getThreads())
        .memory_limit(256 * 1024)
        .debug()
        // .file_upload_target(FILE_UPLOAD_MEMORY_AND_DISK)
    ;

    std::vector<APIRequest *> requests = {
        new LevelAPI::v1::HelloWorldRequest(),
        new LevelAPI::v1::LevelDownloadRequest(),
        new LevelAPI::v1::IMGRequest(),
        new LevelAPI::v1::ResourceRequest(),
        new LevelAPI::v1::StatsRequest(),
        new LevelAPI::v1::LevelSearchRequest(),
        new LevelAPI::v1::IndexRequest(),
        new LevelAPI::v1::IndexRequest2()
    };

    int i = 0;
    while (i < requests.size()) {
        auto req = requests[i];

        ws.register_resource(req->request_url, req->getAsResource());

        i++;
    }

    std::cout << getByKey("lapi.main.portstart", HttpController::getPort()) << std::endl;

    // int i = 0;
    // int size = DatabaseController::database->m_vNodes.size();

    // manager->wipeTable("levels");

    // while (i < size) {
    //     auto node = DatabaseController::database->m_vNodes[i];

    //     std::string levelDir = "database/nodes/" + node->m_sInternalName + "/levels";

    //     DIR *d = opendir(levelDir.c_str());
    //     struct dirent *dir;

    //     while ((dir = readdir(d)) != NULL) {

    //         int id = std::atoi(dir->d_name + 6);
    //         //std::cout << "Updating level " << id << std::endl;
    //         auto level = node->getLevel(id);
    //         if (level != nullptr) {
    //             manager->pushRow({
    //                 {"version", level->m_nVersion},
    //                 {"playerID", level->m_nPlayerID},
    //                 {"downloads", level->m_nDownloads},
    //                 {"musicID", level->m_nMusicID},
    //                 {"likes", level->m_nLikes},
    //                 {"length", level->m_nLength},
    //                 {"difficulty_denominator", level->m_nDifficultyDenominator},
    //                 {"difficulty_numenator", level->m_nDifficultyNumerator},
    //                 {"fakeGameVersion", level->m_nGameVersion},
    //                 {"dislikes", level->m_nDislikes},
    //                 {"stars", level->m_nStars},
    //                 {"featureScore", level->m_nFeatureScore},
    //                 {"copiedFrom", level->m_nCopiedID},
    //                 {"dailyNumber", level->m_nDailyNumber},
    //                 {"coins", level->m_nCoins},
    //                 {"starsRequested", level->m_nStarsRequested},
    //                 {"isEpic", level->m_nEpic},
    //                 {"demonDifficulty", level->m_nDemonDifficulty},
    //                 {"editorTime", level->m_nEditorTime},
    //                 {"editorTimeTotal", level->m_nEditorTimeTotal},
    //                 {"accountID", level->m_nAccountID},
    //                 {"songID", level->m_nSongID},
    //                 {"objects", level->m_nObjects},
    //                 {"moons", level->m_nMoons},
    //                 {"isAuto", level->m_bAuto},
    //                 {"isDemon", level->m_bDemon},
    //                 {"areCoinsVerified", level->m_bVerifiedCoins},
    //                 {"ldmAvailable", level->m_bLDM},
    //                 {"is2P", level->m_b2P},
    //                 {"levelName", level->m_sLevelName},
    //                 {"levelDescription", level->m_sDescription},
    //                 {"uploadDate", level->m_sUploadDate},
    //                 {"updateDate", level->m_sUpdateDate},
    //                 {"username", level->m_sUsername},
    //                 {"actualGameVersion", level->m_uRelease->m_fActualVersion},
    //                 {"databaseAppereanceDate", (uint64_t)level->m_nAppereanceTimestamp},
    //                 {"levelID", level->m_nLevelID}
    //             }, "levels");

    //             std::cout << "Transfered level " << level->m_nLevelID << std::endl;
    //             delete level;
    //             level = nullptr;
    //         }
    //     }
            
    //     closedir(d);

    //     i++;
    // }

    HttpController::generateIndex({"a", "b"});

    ws.start(true);

    return 0;
}
