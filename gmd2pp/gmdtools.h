#pragma once

#include <string>

//#include "curl/include/curl/curl.h"
#include <curl/curl.h>

class GMDTools {
protected:
    std::string m_sURL;

    CURL *m_pInstance;

    bool m_bGotError;

    static size_t f_vWriteData(void *ptr, size_t size, size_t nmemb, FILE *stream);
public:
    std::string &getURL();
    void setURL(std::string &url);

    void downloadFile();

    // read-only
    bool getError();
    
    GMDTools();
    ~GMDTools();
};