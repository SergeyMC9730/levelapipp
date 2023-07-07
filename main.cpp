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

    std::cout << "doing sqlite shit pls wait" << std::endl;

    auto manager = SQLiteManager::create("database/test.db");

    manager->wipeTable("comments");

    int i = 0;
    while(i < 64) {
        manager->pushRow({
            {"message", Backend::ConnectionCrypt::createUUID()},
            {"percentage", 100},
            {"userID", 1},
            {"levelID", 1}
        } , "comments");
        i++;
    }
        
    delete manager;
    // sqlite3_initialize();

    // sqlite3 *sq = NULL;
    // sqlite3_open("database/test.db", &sq);

    // if (!sq) {
    //     std::cout << "error happened!" << std::endl;
    //     return 1;
    // }

    // char *error = NULL;

    // sqlite3_exec(sq, "SELECT * FROM comments", testcallback, sq, &error);

    // if (error) {
    //     std::cout << error << std::endl;
    // }

    // sqlite3_exec(sq, sqlite3_mprintf("DELETE FROM comments WHERE userID = %d", 64), testcallback, sq, &error);

    // if (error) {
    //     std::cout << error << std::endl;
    // }

    // sqlite3_enable_shared_cache(1);
    // sqlite3_config(SQLITE_CONFIG_MULTITHREAD, 1);

    // std::thread d([&]() {
    //     std::cout << "destroying comments" << std::endl;
    //     char *errorlol;
    //     sqlite3_exec(sq, "DELETE FROM comments WHERE 1", testcallback, sq, &errorlol);

    //     std::cout << "creating comments" << std::endl;
    //     int i = 0;
    //     while(i < 1024) {
    //         auto start = std::chrono::high_resolution_clock::now();

    //         char *error = NULL;
    //         std::string message = LevelAPI::Backend::ConnectionCrypt::createUUID();

    //         const char *sql_query = sqlite3_mprintf("INSERT INTO comments (message, percentage, userID, levelID) VALUES ('%s', %d, %d, %d)", message.c_str(), i % 100, i, i * 10);
    //         sqlite3_exec(sq, sql_query, testcallback, sq, &error);
            
    //         if (error) {
    //             std::cout << error << std::endl;
    //             sqlite3_free((void *)error);
    //         }

    //         sqlite3_free((void *)sql_query);

    //         i++;

    //         auto end = std::chrono::high_resolution_clock::now();

    //         auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end - start);
            
    //         std::cout << duration.count() << "micros per query! " << i << std::endl;
    //     }
    // });
    
    // sqlite3_mprintf()
    
    // sqlite3_close(sq);

    // sqlpp::connection con;
    
    // auto config = sqlpp::sqlite3::connection_config("database/test.db");

    // sqlpp::sqlite3::connection con(config);

    // DatabaseController::setup();
    // HttpController::setup();
    // HttpController::parse();

    // LevelAPI::Tests::testCurl();
    // // LevelAPI::Tests::testGDParsers();

    // webserver ws = create_webserver()
    //     .port(HttpController::getPort())
    //     .max_threads(HttpController::getThreads())
    //     .memory_limit(256 * 1024)
    //     .debug()
    //     // .file_upload_target(FILE_UPLOAD_MEMORY_AND_DISK)
    // ;

    // // std::thread t([&]{
    // //     int i = 0;
    // //     int size = DatabaseController::database->m_vNodes.size();
    // //     while(i < size) {
    // //         std::string leveldir = "database/nodes/" + DatabaseController::database->m_vNodes[i]->m_sInternalName + "/levels";
    // //         DIR *d = opendir(leveldir.c_str());
    // //         struct dirent *dir;
    // //         auto nd = DatabaseController::database->getNode("boomlings");
            
    // //         while ((dir = readdir(d)) != NULL) {
    // //             int id = std::atoi(dir->d_name + 6);
    // //             //std::cout << "Updating level " << id << std::endl;
    // //             auto level = nd->getLevel(id);
    // //             if (level != nullptr) {
    // //                 nd->initLevel(level);
    // //                 delete level;
    // //             }
    // //         }
    // //         closedir(d);
    // //         i++;
    // //     }
    // // });

    // // t.detach();

    // ws.register_resource("/api/v1/hello", reinterpret_cast<http_resource *>(new LevelAPI::v1::HelloWorldRequest()));
    // ws.register_resource("/api/v1/level/download", reinterpret_cast<http_resource *>(new LevelAPI::v1::LevelDownloadRequest()));
    // ws.register_resource("/api/v1/img/request/{file}", reinterpret_cast<http_resource *>(new LevelAPI::v1::IMGRequest()));
    // ws.register_resource("/api/v1/res/request/{file}", reinterpret_cast<http_resource *>(new LevelAPI::v1::ResourceRequest()));

    // std::cout << getByKey("lapi.main.portstart", HttpController::getPort()) << std::endl;

    // ws.start(true);

    return 0;
}
