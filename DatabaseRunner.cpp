#include "DatabaseControllerThreads.h"
#include "lapi_database.h"

#include <chrono>
#include <thread>

#include "HttpController.h"

using namespace LevelAPI;
using namespace std::chrono_literals;

void DatabaseController::database_runner(Database *db) {
    while(true) {
        std::this_thread::sleep_for(1s);
        if(db != nullptr) db->save();
        HttpController::save();
    }
}

