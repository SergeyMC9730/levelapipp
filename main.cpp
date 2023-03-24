#include <iostream>

#include <httpserver.hpp>
#include <iterator>
#include "GDServer.h"
#include "requests/v1.helloworld.h"

#include "gmd2pp/gmd2.h"

#include "lapi_database.h"
#include "lapi_version.h"

#include "HttpController.h"

#include "termcolor/include/termcolor/termcolor.hpp"

#include "GDServer_BoomlingsLike21.h"

#include "tests.h"

using namespace LevelAPI;

int main(int, char**) {
    std::cout << termcolor::bright_cyan << "LevelAPI " << LEVELAPI_VERSION << "\n" << termcolor::reset;
    std::cout << termcolor::bright_cyan << "LevelAPI is in alpha state so please report any bugs to my GitHub repository!" << "\n\n" << termcolor::reset;

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
    std::cout << "[LevelAPI] Running on port " << HttpController::getPort() << std::endl;

    DatabaseController::database->save();

    // auto servertest = new Backend::GDServer_BoomlingsLike21(new std::string("123"));
    // servertest->getLevelsBySearch(GDSEARCH_RECENT);

    ws.start(true);

    return 0;
}
