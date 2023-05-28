#include <iostream>

#include <httpserver.hpp>
#include <iterator>
#include "GDServer.h"
#include "Level.h"

#include "requests/v1.helloworld.h"
#include "requests/v1.level.download.h"
#include "requests/v1.img.request.h"
#include "requests/v1.res.request.h"

#include "gmd2pp/gmd2.h"

#include "lapi_database.h"
#include "lapi_version.h"

#include "HttpController.h"

#include "GDServer_BoomlingsLike21.h"

#include "tests.h"

#include "Translation.h"

using namespace LevelAPI;
using namespace LevelAPI::Frontend::Translation;

int main(int, char**) {
    std::cout << "LevelAPI " << LEVELAPI_VERSION << "\n";
    std::cout << getByKey("lapi.main.alpha") << "\n\n";

    DatabaseController::setup();
    HttpController::setup();
    HttpController::parse();

    LevelAPI::Tests::testCurl();
    // LevelAPI::Tests::testGDParsers();

    webserver ws = create_webserver()
        .port(HttpController::getPort())
        .max_threads(HttpController::getThreads())
        .memory_limit(256 * 1024)
        .debug()
        // .file_upload_target(FILE_UPLOAD_MEMORY_AND_DISK)
    ;

    ws.register_resource("/api/v1/hello", reinterpret_cast<http_resource *>(new LevelAPI::v1::HelloWorldRequest()));
    ws.register_resource("/api/v1/level/download", reinterpret_cast<http_resource *>(new LevelAPI::v1::LevelDownloadRequest()));
    ws.register_resource("/api/v1/img/request/{file}", reinterpret_cast<http_resource *>(new LevelAPI::v1::IMGRequest()));
    ws.register_resource("/api/v1/res/request/{file}", reinterpret_cast<http_resource *>(new LevelAPI::v1::ResourceRequest()));

    std::cout << getByKey("lapi.main.portstart", HttpController::getPort()) << std::endl;

    ws.start(true);

    return 0;
}
