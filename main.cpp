#include <iostream>

#include <httpserver.hpp>
#include <iterator>
#include "requests/v1.helloworld.h"

#include "gmd2pp/gmd2.h"

#include "lapi_database.h"
#include "lapi_version.h"

using namespace LevelAPI;

int main(int, char**) {
    std::cout << "LevelAPI " << LEVELAPI_VERSION << std::endl;

    DatabaseController::setup();
    DatabaseController::HttpController::parse();

    webserver ws = create_webserver()
        .port(DatabaseController::HttpController::getPort())
        .max_threads(64)
        .memory_limit(256 * 1024)
        .debug()
        .file_upload_target(FILE_UPLOAD_MEMORY_AND_DISK);

    LevelAPI::v1::HelloWorldRequest hwr;

    ws.register_resource(hwr.request_url, &hwr);
    std::cout << "[LevelAPI] Running on port " << DatabaseController::HttpController::getPort() << std::endl;

    GMD2 *gm = new GMD2();

    gm->setFileName("Level_2482.gmd2");
    gm->setDebug(true);
    gm->parse();

    auto db = DatabaseController::Database();
    db.m_vNodes.push_back(DatabaseController::Node(DatabaseController::NodeDatabase("123", 21, false), "AAA", "AAA/"));
    db.m_vNodes.push_back(DatabaseController::Node(DatabaseController::NodeDatabase("123", 21, false), "BBB", "BBB/"));

    DatabaseController::Database aaa;

    ws.start(true);

    return 0;
}
