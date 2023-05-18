#include "HttpController.h"

#include "json/single_include/nlohmann/json.hpp"

using namespace LevelAPI;

#include <string>
#include <fstream>
#include <sstream>
#include <thread>

nlohmann::json HttpController_json;

void HttpController::parse() {
    std::ifstream h("database/configuration/http.json");
    HttpController_json = nlohmann::json::parse(h);
    
}
int HttpController::getPort() {
    if(HttpController_json.contains("port")) {
        return HttpController_json["port"].get<int>();
    } else {
        return 8000;
    }
}
int HttpController::getThreads() {
    if (HttpController_json.contains("threads")) {
        return HttpController_json["threads"].get<int>();
    } else {
        return std::thread::hardware_concurrency();
    }
}
std::string HttpController::getURL() {
    if (HttpController_json.contains("url")) {
        return HttpController_json["url"].get<std::string>();
    } else {
        return "https://levelapi.dogotrigger.xyz";
    }
}

void HttpController::setup() {
    std::ifstream h("database/configuration/http.json");
    if(!h.good()) {
        nlohmann::json j;
        j["port"] = 8000;
        j["threads"] = std::thread::hardware_concurrency();
        j["url"] = "https://levelapi.dogotrigger.xyz";

        std::string j2 = j.dump();

        std::ofstream j3;
        j3.open ("database/configuration/http.json");
        j3 << j2;
        j3.close();
    }
}
void HttpController::save() {
    HttpController_json["port"] = getPort();
    HttpController_json["threads"] = getThreads();
    HttpController_json["url"] = getURL();

    std::string j2 = HttpController_json.dump();

    std::ofstream j3;
    j3.open ("database/configuration/http.json");
    j3 << j2;
    j3.close();
}