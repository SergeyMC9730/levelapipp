#include "lapi_database.h"

#include <iostream>
#include <sys/stat.h>
#include <vector>
using namespace std;

void LevelAPI::DatabaseController::setup() {
    std::vector<std::string> fpaths = {
        "database", 
        "database/configuration", 
        "database/nodes"
    };

    int i = 0;
    while(i < fpaths.size()) {
        mkdir(fpaths[i].c_str(), 777);
        i++;
    }
}