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

#pragma once

#include <curl/curl.h>
#include <string>
#include <vector>

namespace LevelAPI {
    namespace Backend {
        enum ProxyType {
            PT_NONE = 0, 
            PT_HTTP, PT_HTTPS, PT_SOCKS4, PT_SOCKS5
        };

        class CurlProxy {
        protected:
            ProxyType m_eType = PT_NONE;
            std::string m_sURL = "";
        public:
            CurlProxy();

            virtual std::string getURL();
            virtual ProxyType getType();

            static ProxyType fromString(std::string url);

            CurlProxy(std::string url);
            CurlProxy(const char * url);
        };
        class CURLParameter {
        public:
            CURLParameter();
            CURLParameter(std::string k, std::string v);
            CURLParameter(std::string k, int v);

            std::string key;
            std::string value;
        };
        class CURLResult {
        public:
            int realSize;

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
            CurlProxy m_proxy;

            void destroy();
        public:
            void setDebug(bool d);
            void setData(std::vector<CURLParameter*> parameters);
            void setProxy(CurlProxy proxy);

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