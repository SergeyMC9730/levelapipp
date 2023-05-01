#include <iostream>

#include <httpserver.hpp>
#include <iterator>
#include "GDServer.h"
#include "Tools.h"
#include "requests/v1.helloworld.h"
#include "requests/v1.level.download.h"

#include "gmd2pp/gmd2.h"

#include "lapi_database.h"
#include "lapi_version.h"

#include "HttpController.h"

#include "termcolor/include/termcolor/termcolor.hpp"

#include "GDServer_BoomlingsLike21.h"

#include "tests.h"

#include "Translation.h"

using namespace LevelAPI;
using namespace LevelAPI::Frontend::Translation;

int main(int, char**) {
    std::cout << termcolor::bright_cyan << "LevelAPI " << LEVELAPI_VERSION << "\n" << termcolor::reset;
    std::cout << termcolor::bright_cyan << getByKey("lapi.main.alpha") << "\n\n" << termcolor::reset;

    DatabaseController::setup();
    HttpController::setup();
    HttpController::parse();

    std::cout << termcolor::reset;

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

    std::cout << getByKey("lapi.main.portstart", HttpController::getPort()) << std::endl;

    DatabaseController::database->save();

    ws.start(true);

    return 0;
}
