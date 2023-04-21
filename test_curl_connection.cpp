#include "CURLParameter.h"
#include "curl_backend.h"
#include "tests.h"

#include <iostream>

#include <thread>
#include "termcolor/include/termcolor/termcolor.hpp"

#include "Translation.h"

using namespace LevelAPI;
using namespace LevelAPI::Frontend;

void Tests::testCurl() {
    std::cout << termcolor::bright_cyan << Translation::getByKey("lapi.curltest.start") << termcolor::reset;

    std::thread tr(Tests::testCurlThread);
    tr.detach();

    return;
}
void Tests::testCurlThread() {
    Backend::CURLConnection con = Backend::CURLConnection();
    con.setDebug(false);
    Backend::CURLResult *res = con.access_page("https://www.google.com");

    printf(Translation::getByKey("lapi.curltest.test0result").c_str(), res->result, res->http_status);
    if(res->http_status == 200 && res->result == 0) {
        std::cout << termcolor::green << Translation::getByKey("lapi.curltest.complete") << termcolor::reset;
    } else {
        std::cout << termcolor::red << Translation::getByKey("lapi.curltest.fail") << termcolor::reset;
    }

    con.setData({
        new CURLParameter("secret", "Wmfd2893gb7"),
        new CURLParameter("type", "4")
    });

    free((void *)res->data);
    delete res;
    res = nullptr;

    return;
}