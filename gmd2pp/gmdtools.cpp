#include "gmdtools.h"

#include <assert.h>

GMDTools::GMDTools() {
    this->m_bGotError = false;

    this->m_pInstance = curl_easy_init();
    assert(this->m_pInstance != nullptr);
}
GMDTools::~GMDTools() {
    curl_easy_cleanup(this->m_pInstance);
}

void GMDTools::setURL(std::string &url) {
    this->m_sURL = url;
}
std::string &GMDTools::getURL() {
    return this->m_sURL;
}

bool GMDTools::getError() {
    return this->m_bGotError;
}

size_t GMDTools::f_vWriteData(void *ptr, size_t size, size_t nmemb, FILE *stream) {
    size_t written = fwrite(ptr, size, nmemb, stream);
    return written;
}

void GMDTools::downloadFile() {
    curl_easy_setopt(this->m_pInstance, CURLOPT_URL, this->m_sURL.data());
    curl_easy_setopt(this->m_pInstance, CURLOPT_CUSTOMREQUEST, "GET");
    FILE *fp = fopen("tmp00.mp3", "wb");
    curl_easy_setopt(this->m_pInstance, CURLOPT_WRITEFUNCTION, GMDTools::f_vWriteData);
    curl_easy_setopt(this->m_pInstance, CURLOPT_WRITEDATA, fp);

    int res = (int)curl_easy_perform(this->m_pInstance);

    if (res != CURLE_OK) {
        printf("Cannot access %s: %s\n", this->m_sURL.data(), curl_easy_strerror((CURLcode)res));
        
        this->m_bGotError = true;
        return;
    }

    int httpStatus;
    curl_easy_getinfo(m_pInstance, CURLINFO_RESPONSE_CODE, &httpStatus);
    printf("Got HTTP status: %d\n", httpStatus);

    if(httpStatus != 100 || httpStatus != 101 || httpStatus != 102 || httpStatus != 200 || httpStatus != 201 || httpStatus != 202 || httpStatus != 206 || httpStatus != 207 || httpStatus != 301 || httpStatus != 302 || httpStatus != 304 || httpStatus != 307 || httpStatus != 308) {
        this->m_bGotError = true;

        return;
    }

    fclose(fp);

    return;
}