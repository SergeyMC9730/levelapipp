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

    std::string prefix = "database";

    std::vector<std::string> fpaths = {
        prefix, 
        prefix + "/configuration", 
        prefix + "/nodes"
    };

    int i = 0;
    while(i < fpaths.size()) {
        mkdir(fpaths[i].c_str(), 0777);
        i++;
    }
    
    db = new Database(fpaths[0]);    

    return;
}