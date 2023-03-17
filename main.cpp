#include <iostream>

#include <httpserver.hpp>
#include <iterator>
#include "requests/v1.helloworld.h"

#include "gmd2pp/gmd2.h"

#include "lapi_database.h"
#include "lapi_version.h"

#include "termcolor/include/termcolor/termcolor.hpp"

#include "tests.h"

using namespace LevelAPI;

int main(int, char**) {
    std::cout << termcolor::bright_cyan << "LevelAPI " << LEVELAPI_VERSION << "\n\n" << termcolor::reset;

    DatabaseController::setup();
    DatabaseController::HttpController::parse();

    std::cout << termcolor::reset;

    LevelAPI::Tests::testCurl();

    webserver ws = create_webserver()
        .port(DatabaseController::HttpController::getPort())
        .max_threads(64)
        .memory_limit(256 * 1024)
        .debug()
        // .file_upload_target(FILE_UPLOAD_MEMORY_AND_DISK)
    ;

    ws.register_resource("/api/v1/hello", static_cast<http_resource *>(new LevelAPI::v1::HelloWorldRequest()));
    std::cout << "[LevelAPI] Running on port " << DatabaseController::HttpController::getPort() << std::endl;

    DatabaseController::database->save();

    ws.start(true);

    return 0;
}
