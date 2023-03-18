#include "lapi_database.h"

#include <iostream>
#include <sys/stat.h>
#include <vector>
#include <fstream>

#include "json/single_include/nlohmann/json.hpp"

using namespace std;

LevelAPI::DatabaseController::Database *LevelAPI::DatabaseController::database;

void LevelAPI::DatabaseController::setup() {
    #define db LevelAPI::DatabaseController::database

    std::vector<std::string> fpaths = {
        "database", 
        "database/configuration", 
        "database/nodes"
    };

    int i = 0;
    while(i < fpaths.size()) {
        mkdir(fpaths[i].c_str(), 0777);
        i++;
    }

    nlohmann::json j;
    j["port"] = 8000;

    std::string j2 = j.dump();

    ofstream j3;
    j3.open ("database/configuration/http.json");
    j3 << j2;
    j3.close();

    db = new Database(new std::string("database"));    

    return;
}