#include "lapi_database.h"

#include "json/single_include/nlohmann/json.hpp"

using namespace LevelAPI::DatabaseController;

#include <string>
#include <fstream>
#include <sstream>

nlohmann::json HttpController_json;

void HttpController::parse() {
    std::ifstream h("database/configuration/http.json");
    HttpController_json = nlohmann::json::parse(h);
    
}
int HttpController::getPort() {
    return HttpController_json["port"];
}