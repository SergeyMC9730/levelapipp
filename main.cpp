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
#include "requests/v1.helloworld.h"
#include "requests/v1.level.download.h"
#include "requests/v1.img.request.h"
#include "requests/v1.res.request.h"

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
        "boomlings2.2"
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
        LevelAPI::Tests::test_boomlings_ver22();
    }

    return;
}

std::vector<std::string> getArguments(int argc, char *argv[]) {
    if (argc == 1) return {};

    int i = 1;
    std::vector<std::string> arguments = {};

    while(i < argc) {
        arguments.push_back(argv[i]);
        i++;
    }

    return arguments;
}

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

int main(int argc, char *argv[]) {
    std::cout << "LevelAPI " << LEVELAPI_VERSION << "\n";
    std::cout << getByKey("lapi.main.alpha") << "\n\n";

    srand(time(nullptr));

    auto args = getArguments(argc, argv);

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

        std::cout << "Help:\n - test  -> testing new LevelAPI features\n";

        return 1;
    }

    auto manager = SQLiteManager::create("database/test.db");

    manager->updateRow("comments", {
        {"message", "hi"},
        {"percentage", 100},
        {"userID", 1},
        {"levelID", 1}
    }, {
        {"message", "hi"}
    });

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

    delete manager;

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

    ws.register_resource("/api/v1/hello", reinterpret_cast<http_resource *>(new LevelAPI::v1::HelloWorldRequest()));
    ws.register_resource("/api/v1/level/download", reinterpret_cast<http_resource *>(new LevelAPI::v1::LevelDownloadRequest()));
    ws.register_resource("/api/v1/img/request/{file}", reinterpret_cast<http_resource *>(new LevelAPI::v1::IMGRequest()));
    ws.register_resource("/api/v1/res/request/{file}", reinterpret_cast<http_resource *>(new LevelAPI::v1::ResourceRequest()));

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

    ws.start(true);

    return 0;
}