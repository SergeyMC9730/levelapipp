#include "CURLParameter.h"
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

    printf("[LevelAPI TEST 0] Google returned %d %d\n", res->result, res->http_status);
    if(res->http_status == 200 && res->result == 0) {
        std::cout << termcolor::green << "[LevelAPI] cURL connection test complete\n" << termcolor::reset;
    } else {
        std::cout << termcolor::red << "[LevelAPI] cURL connection test FAILED\n[LevelAPI] Don't expect connection backend to work!\n" << termcolor::reset;
    }

    con.setData({
        new CURLParameter("secret", "Wmfd2893gb7"),
        new CURLParameter("type", "4")
    });

    free((void *)res->data);
    delete res;
    res = nullptr;

    // res = con.access_page("https://www.boomlings.com/database/getGJLevels21.php", "POST");

    // printf("[LevelAPI TEST 0] Boomlings offical returned %d %d\n", res->result, res->http_status);
    // if(res->http_status == 200 && res->result == 0) {
    //     std::cout << termcolor::green << "[LevelAPI] cURL connection test complete" << std::endl << termcolor::reset;
    // } else {
    //     std::cout << termcolor::red << "[LevelAPI] cURL connection test FAILED\n[LevelAPI] Don't expect connection backend to work!\n" << termcolor::reset;
    // }

    // free((void *)res->data);
    // delete res;
    // res = nullptr;

    return;
}