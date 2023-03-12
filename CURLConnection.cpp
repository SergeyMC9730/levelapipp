#include "curl_backend.h"
#include "gmd2pp/curl/include/curl/easy.h"
#include <cstddef>
#include <cstdlib>
#include <curl/curl.h>
#include <stdio.h>

using namespace LevelAPI::Backend;

CURLConnection::CURLConnection() {
    m_pCurl = curl_easy_init();
}
CURLConnection::~CURLConnection() {
    curl_easy_cleanup(m_pCurl);
}

size_t CURLConnection::write_data(void *ptr, size_t size, size_t nmemb, void *stream) {
    auto settings = static_cast<CURLConnectionSettings *>(stream);

    if(settings->m_bWriteToMemory) {
        if(settings->sendDebug) printf("size: %d nmemb: %d | msize: %d mx size: %d\n", (int)size, (int)nmemb, settings->m_nSize, settings->m_nMaxMemorySize);
        if(nmemb > settings->m_nMaxMemorySize - settings->m_nSize) {
            settings->m_pData = realloc(settings->m_pData, settings->m_nMaxMemorySize + nmemb);
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