#include "HttpController.h"

#include "json/single_include/nlohmann/json.hpp"

using namespace LevelAPI;

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
int HttpController::getThreads() {
    return HttpController_json["threads"];
}
void HttpController::setup() {
    std::ifstream h("database/configuration/http.json");
    if(!h.good()) {
        nlohmann::json j;
        j["port"] = 8000;
        j["threads"] = 4;

        std::string j2 = j.dump();

        std::ofstream j3;
        j3.open ("database/configuration/http.json");
        j3 << j2;
        j3.close();
    }
}