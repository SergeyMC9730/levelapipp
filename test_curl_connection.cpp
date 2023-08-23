/**
 *  LevelAPI - Geometry Dash level cacher with search functionality and more.
    Copyright (C) 2023  Sergei Baigerov

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <https://www.gnu.org/licenses/>.
*/

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