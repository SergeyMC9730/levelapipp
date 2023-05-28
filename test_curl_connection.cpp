#include "CURLParameter.h"
#include "curl_backend.h"
#include "tests.h"

#include <iostream>

#include <thread>

#include "Translation.h"

using namespace LevelAPI;
using namespace LevelAPI::Frontend;

void Tests::testCurl() {
    std::cout << Translation::getByKey("lapi.curltest.start");

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
        std::cout << Translation::getByKey("lapi.curltest.complete");
    } else {
        std::cout << Translation::getByKey("lapi.curltest.fail");
    }

    free((void *)res->data);
    delete res;
    res = nullptr;

    return;
}