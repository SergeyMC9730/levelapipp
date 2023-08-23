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
#include <cstddef>
#include <cstdlib>
#include <curl/curl.h>
#include <iostream>
#include <algorithm>
#include "Translation.h"

using namespace LevelAPI::Backend;
using namespace LevelAPI::Frontend;

CURLConnection::CURLConnection() {
    m_pCurl = curl_easy_init();
}

size_t CURLConnection::write_data(void *ptr, size_t size, size_t nmemb, void *stream) {
    auto settings = static_cast<CURLConnectionSettings *>(stream);

    if(settings->m_bWriteToMemory) {
        //if(settings->sendDebug) printf("size: %d nmemb: %d | msize: %d mx size: %d\n", (int)size, (int)nmemb, settings->m_nSize, settings->m_nMaxMemorySize);
        if(nmemb > settings->m_nMaxMemorySize - settings->m_nSize) {
            settings->m_pData = realloc(settings->m_pData, settings->m_nMaxMemorySize + nmemb + 1);
            settings->m_nMaxMemorySize = settings->m_nMaxMemorySize + nmemb;
        }
        int i = 0;
        char *dd = (char *)settings->m_pData;
        char *d1 = (char *)ptr;
        while(i < nmemb) {
            dd[settings->m_nSize + i] = d1[i];
            i++;  
        }
        settings->m_nSize += nmemb;
        return nmemb;
    } else {
        size_t written = fwrite(ptr, size, nmemb, settings->m_pFile);
        settings->m_nSize += written;
        return written;
    }
}

CURLResult *CURLConnection::access_page(const char *url) {
    return access_page(url, "GET");
}
CURLResult *CURLConnection::access_page(const char *url, const char *method) {
    CURLConnectionSettings *settings = new CURLConnectionSettings(malloc(1024), 1024);
    CURLResult *res = new CURLResult();

    curl_easy_setopt(m_pCurl, CURLOPT_URL, url);
    curl_easy_setopt(m_pCurl, CURLOPT_CUSTOMREQUEST, method);
    curl_easy_setopt(m_pCurl, CURLOPT_WRITEFUNCTION, CURLConnection::write_data);
    curl_easy_setopt(m_pCurl, CURLOPT_WRITEDATA, settings);
    curl_easy_setopt(m_pCurl, CURLOPT_VERBOSE, m_bDebug);
    curl_easy_setopt(m_pCurl, CURLOPT_TIMEOUT, 10);
    if(m_proxy.getType() != PT_NONE) {
        curl_easy_setopt(m_pCurl, CURLOPT_PROXY, m_proxy.getURL().c_str());
    }

    std::string generatedUserData = "";
    int i = 0;
    while(i < m_mUserData.size()) {
        generatedUserData += m_mUserData[i]->key.c_str();
        generatedUserData += "=";
        generatedUserData += m_mUserData[i]->value.c_str();
        if(i + 1 != m_mUserData.size()) {
            generatedUserData += "&";
        }
        i++;
    }
    std::replace(generatedUserData.begin(), generatedUserData.end(), ' ', '+');

    if(generatedUserData.compare("") != 0) {
        curl_easy_setopt(m_pCurl, CURLOPT_POSTFIELDS, generatedUserData.c_str());
    }

    int result = (int)curl_easy_perform(m_pCurl);

    if(result != CURLE_OK) {
        //printf("[CURL WARN] %s\n", curl_easy_strerror((CURLcode)result));
        std::cout << Translation::getByKey("curl.event.warning.connection", std::string(curl_easy_strerror((CURLcode)(result)))) << std::endl;
        res->result = result;
        res->data = (const char *)settings->m_pData;
        res->maxSize = 1024;
        return res;
    }

    int httpStatus;
    curl_off_t ra;
    curl_easy_getinfo(m_pCurl, CURLINFO_RESPONSE_CODE, &httpStatus);
    curl_easy_getinfo(m_pCurl, CURLINFO_RETRY_AFTER, &ra);
    res->result = result;
    res->http_status = httpStatus;
    res->retry_after = (int)ra;
    // printf("[CURL] retry after %d seconds\n", (int)ra);
    res->data = (const char *)settings->m_pData;
    res->maxSize = settings->m_nMaxMemorySize;
    res->realSize = settings->m_nSize;

    delete settings;
    settings = nullptr;

    char *resdata = (char *)res->data;
    resdata[res->realSize] = 0;
    
    return res;
}
CURLResult *CURLConnection::access_page(const char *url, FILE *f) {
    return access_page(url, "GET", f);
}
CURLResult *CURLConnection::access_page(const char *url, const char *method, FILE *f) {
    CURLConnectionSettings *settings = new CURLConnectionSettings(f);
    settings->sendDebug = m_bDebug;
    CURLResult *res = new CURLResult();

    curl_easy_setopt(m_pCurl, CURLOPT_URL, url);
    curl_easy_setopt(m_pCurl, CURLOPT_CUSTOMREQUEST, method);
    curl_easy_setopt(m_pCurl, CURLOPT_WRITEFUNCTION, CURLConnection::write_data);
    curl_easy_setopt(m_pCurl, CURLOPT_WRITEDATA, settings);
    curl_easy_setopt(m_pCurl, CURLOPT_VERBOSE, true);
    curl_easy_setopt(m_pCurl, CURLOPT_FOLLOWLOCATION, true);
    curl_easy_setopt(m_pCurl, CURLOPT_HTTPPROXYTUNNEL, true);
    curl_easy_setopt(m_pCurl, CURLOPT_TIMEOUT, 10);
    if(m_proxy.getType() != PT_NONE) {
        curl_easy_setopt(m_pCurl, CURLOPT_PROXY, m_proxy.getURL().c_str());
    }

    std::string generatedUserData = "";
    int i = 0;
    while(i < m_mUserData.size()) {
        // if(true) printf("param %d: %s=%s\n", i, m_mUserData[i]->key.c_str(), m_mUserData[i]->value.c_str());
        generatedUserData += m_mUserData[i]->key.c_str();
        generatedUserData += "=";
        generatedUserData += m_mUserData[i]->value.c_str();
        if(i + 1 != m_mUserData.size()) {
            generatedUserData += "&";
        }
        i++;
    }
    std::replace(generatedUserData.begin(), generatedUserData.end(), ' ', '+');

    if(generatedUserData.compare("") != 0) {
        curl_easy_setopt(m_pCurl, CURLOPT_POSTFIELDS, generatedUserData.c_str());
    }

    int result = (int)curl_easy_perform(m_pCurl);

    if(result != CURLE_OK) {
        std::cout << Translation::getByKey("curl.event.warning.connection", std::string(curl_easy_strerror((CURLcode)(result)))) << std::endl;
        res->result = result;
        res->data = (const char *)settings->m_pData;
        res->maxSize = 1024;
        return res;
    }

    int httpStatus;
    curl_easy_getinfo(m_pCurl, CURLINFO_RESPONSE_CODE, &httpStatus);
    res->result = result;
    res->http_status = httpStatus;
    res->data = (const char *)settings->m_pData;
    res->maxSize = settings->m_nSize;

    delete settings;
    settings = nullptr;
    
    return res;
}

void CURLConnection::setDebug(bool d) {
    m_bDebug = d;
}

void CURLConnection::destroy() {
    curl_easy_cleanup(m_pCurl);
}

void CURLConnection::setProxy(CurlProxy proxy) {
    m_proxy = proxy;
}

void CURLConnection::setData(std::vector<CURLParameter*> parameters) {
    int i = 0;
    
    while(i < m_mUserData.size()) {
        delete m_mUserData[i];
        m_mUserData[i] = nullptr;
        i++;
    }
    m_mUserData.clear();

    this->m_mUserData = parameters;
}

CURLConnection::~CURLConnection() {
    int i = 0;

    while(i < m_mUserData.size()) {
        delete m_mUserData[i];
        m_mUserData[i] = nullptr;
        i++;
    }
    m_mUserData.clear();

    destroy();
}
