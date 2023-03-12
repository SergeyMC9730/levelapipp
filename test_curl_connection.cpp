#include "curl_backend.h"
#include "tests.h"

#include <iostream>

#include <thread>
#include "termcolor/include/termcolor/termcolor.hpp"

using namespace LevelAPI;

void Tests::testCurl() {
    std::cout << termcolor::bright_cyan << "[LevelAPI] Running cURL connection test...\n" << termcolor::reset;

    std::thread tr(Tests::testCurlThread);
    tr.detach();

    return;
}
void Tests::testCurlThread() {
    Backend::CURLConnection con = Backend::CURLConnection();
    con.setDebug(false);
    Backend::CURLResult *res = con.access_page("https://www.google.com");

    printf("[LevelAPI TEST 0] %d %d\n", res->result, res->http_status);

    delete res;

    std::cout << termcolor::green << "[LevelAPI] cURL connection test complete\n" << termcolor::reset;
    //printf("%s\n", res->data);

    return;
}