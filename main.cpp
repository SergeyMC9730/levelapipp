#include <iostream>

#include <httpserver.hpp>
#include "requests/v1.helloworld.h"

#include "gmd2pp/gmd2.h"

uint16_t port = 8000;

int main(int, char**) {
    std::cout << "Hello, world!\n";

    webserver ws = create_webserver()
        .port(port)
        .max_threads(64)
        .memory_limit(256 * 1024)
        .debug()
        .file_upload_target(FILE_UPLOAD_MEMORY_AND_DISK);

    LevelAPI::v1::HelloWorldRequest hwr;

    ws.register_resource(hwr.request_url, &hwr);
    std::cout << "[LevelAPI] Running on port " << port << std::endl;

    GMD2 *gm = new GMD2();

    gm->setFileName("test.gmd2");
    gm->setDebug(true);
    gm->parse();

    ws.start(true);

    return 0;
}
