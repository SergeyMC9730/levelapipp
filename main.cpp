#include <iostream>

#include <httpserver.hpp>
#include <iterator>
#include "requests/v1.helloworld.h"

#include "gmd2pp/gmd2.h"

#include "lapi_database.h"
#include "lapi_version.h"

#include "termcolor/include/termcolor/termcolor.hpp"

using namespace LevelAPI;

int main(int, char**) {
    std::cout << termcolor::bright_cyan << "LevelAPI " << LEVELAPI_VERSION << "\n\n" << termcolor::reset;

    DatabaseController::setup();
    DatabaseController::HttpController::parse();

    webserver ws = create_webserver()
        .port(DatabaseController::HttpController::getPort())
        .max_threads(64)
        .memory_limit(256 * 1024)
        .debug()
        // .file_upload_target(FILE_UPLOAD_MEMORY_AND_DISK)
    ;
    LevelAPI::v1::HelloWorldRequest hwr;

    ws.register_resource(hwr.request_url, &hwr);
    std::cout << "[LevelAPI] Running on port " << DatabaseController::HttpController::getPort() << std::endl;

    DatabaseController::database->save();

    ws.start(true);

    return 0;
}
