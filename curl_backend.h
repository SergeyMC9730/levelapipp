#pragma once

#include <curl/curl.h>

namespace LevelAPI {
    namespace Backend {
        class CURLResult {
        public:
            int result;
            int maxSize;

            int http_status;

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
        };
        class CURLConnection {
        protected:
            CURL *m_pCurl;
            bool m_bDebug;
        public:
            void setDebug(bool d);

            virtual CURLResult *access_page(const char *url);
            virtual CURLResult *access_page(const char *url, FILE *file);
            virtual CURLResult *access_page(const char *url, const char *method);
            virtual CURLResult *access_page(const char *url, const char *method, FILE *file);
            
            static size_t write_data(void *ptr, size_t size, size_t nmemb, void *stream);

            void destroy();
        
            CURLConnection();
            ~CURLConnection();
        };
    }
}