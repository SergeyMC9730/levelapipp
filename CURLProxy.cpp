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

using namespace LevelAPI::Backend;

CurlProxy::CurlProxy() {
    m_sURL = "";
    m_eType = PT_NONE;
}

CurlProxy::CurlProxy(std::string url) {
    m_sURL = url;
    m_eType = CurlProxy::fromString(url);
}

ProxyType CurlProxy::fromString(std::string url) {
    ProxyType type;

    // https://curl.se/libcurl/c/CURLOPT_PROXY.html

    if (url.find("http://") != std::string::npos) {
        type = PT_HTTP;
    } else if (url.find("https://") != std::string::npos) {
        type = PT_HTTPS;
    } else if (url.find("socks4://") != std::string::npos) {
        type = PT_SOCKS4;
    } else if (url.find("socks5://") != std::string::npos) {
        type = PT_SOCKS5;
    } else {
        type = PT_NONE;
    }

    return type;
}

CurlProxy::CurlProxy(const char *url) {
    m_sURL = url;
    m_eType = CurlProxy::fromString(m_sURL);
}

std::string CurlProxy::getURL() {
    return m_sURL;
}
ProxyType CurlProxy::getType() {
    return m_eType;
}