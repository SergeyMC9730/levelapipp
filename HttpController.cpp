#include "lapi_database.h"



using namespace LevelAPI::DatabaseController;

#include <string>
#include <fstream>
#include <sstream>

void HttpController::parse() {
    std::ifstream t("database/configuration/http.json");
    std::stringstream buffer;
    buffer << t.rdbuf();
}
int HttpController::getPort() {
    return 8000;
}