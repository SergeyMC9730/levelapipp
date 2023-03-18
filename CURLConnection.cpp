#include "curl_backend.h"
#include "gmd2pp/curl/include/curl/easy.h"
#include <cstddef>
#include <cstdlib>
#include <curl/curl.h>
#include <stdio.h>
#include <algorithm>

using namespace LevelAPI::Backend;

CURLConnection::CURLConnection() {
    m_pCurl = curl_easy_init();
}
CURLConnection::~CURLConnection() {
    printf("~curlconnection\n");
    int i = 0;
    while(i < m_mUserData.size()) {
        delete m_mUserData[i];
        i++;
    }
    return;
}

size_t CURLConnection::write_data(void *ptr, size_t size, size_t nmemb, void *stream) {
    auto settings = static_cast<CURLConnectionSettings *>(stream);

    if(settings->m_bWriteToMemory) {
        if(settings->sendDebug) printf("size: %d nmemb: %d | msize: %d mx size: %d\n", (int)size, (int)nmemb, settings->m_nSize, settings->m_nMaxMemorySize);
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

    std::string generatedUserData = "";
    int i = 0;
    while(i < m_mUserData.size()) {
        if(m_bDebug) {
            printf("key addr at %p, val addr at %p\n", m_mUserData[i]->key, m_mUserData[i]->value);
            printf("param %d: %s=%s\n", i, m_mUserData[i]->key, m_mUserData[i]->value);
        }
        generatedUserData += m_mUserData[i]->key;
        generatedUserData += "=";
        generatedUserData += m_mUserData[i]->value;
        if(i + 1 != m_mUserData.size()) {
            generatedUserData += "&";
        }
        i++;
    }
    std::replace(generatedUserData.begin(), generatedUserData.end(), ' ', '+');

    if(generatedUserData.compare("") != 0) {
        if(m_bDebug) printf("Parameters: %s\n", generatedUserData.c_str());
        curl_easy_setopt(m_pCurl, CURLOPT_POSTFIELDS, generatedUserData.c_str());
    }

    int result = (int)curl_easy_perform(m_pCurl);

    if(result != CURLE_OK) {
        printf("[CURL WARN] Couldn't connect to server: %s\n", curl_easy_strerror((CURLcode)result));
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

    std::string generatedUserData = "";
    int i = 0;
    while(i < m_mUserData.size()) {
        if(m_bDebug) printf("param %d: %s=%s\n", i, m_mUserData[i]->key, m_mUserData[i]->value);
        generatedUserData += m_mUserData[i]->key;
        generatedUserData += "=";
        generatedUserData += m_mUserData[i]->value;
        if(i + 1 != m_mUserData.size()) {
            generatedUserData += "&";
        }
        i++;
    }
    std::replace(generatedUserData.begin(), generatedUserData.end(), ' ', '+');

    if(generatedUserData.compare("") != 0) {
        if(m_bDebug) printf("Parameters: %s\n", generatedUserData.c_str());
        curl_easy_setopt(m_pCurl, CURLOPT_POSTFIELDS, generatedUserData.c_str());
    }

    int result = (int)curl_easy_perform(m_pCurl);

    if(result != CURLE_OK) {
        printf("[CURL WARN] Couldn't connect to server: %s\n", curl_easy_strerror((CURLcode)result));
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
    
    return res;
}

void CURLConnection::setDebug(bool d) {
    m_bDebug = d;
}

void CURLConnection::destroy() {
    //curl_easy_cleanup(m_pCurl);
}

void CURLConnection::setData(std::vector<CURLParameter*> parameters) {
    this->m_mUserData = parameters;
}