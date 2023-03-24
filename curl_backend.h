#pragma once

#include <curl/curl.h>
#include <string>
#include "CURLParameter.h"
#include <vector>

namespace LevelAPI {
    namespace Backend {
        class CURLResult {
        public:
            int result;
            int maxSize;

            int http_status;
            int retry_after;

            const char *data;
        };
        class CURLConnectionSettings {
        public:
            bool m_bWriteToMemory;

            FILE *m_pFile;
            void *m_pData;

            int m_nMaxMemorySize;
            int m_nSize;

            bool sendDebug;

            CURLConnectionSettings(FILE *file);
            CURLConnectionSettings(void *memory_pointer, int maxSize);

            ~CURLConnectionSettings();
        };
        class CURLConnection {
        protected:
            CURL *m_pCurl;
            bool m_bDebug;
            std::vector<CURLParameter*> m_mUserData;

            void destroy();
        public:
            void setDebug(bool d);
            void setData(std::vector<CURLParameter*> parameters);

            virtual CURLResult *access_page(const char *url);
            virtual CURLResult *access_page(const char *url, FILE *file);
            virtual CURLResult *access_page(const char *url, const char *method);
            virtual CURLResult *access_page(const char *url, const char *method, FILE *file);
            
            static size_t write_data(void *ptr, size_t size, size_t nmemb, void *stream);
        
            CURLConnection();
            ~CURLConnection();
        };
    }
}