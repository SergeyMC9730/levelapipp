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
        // supported proxies by cURL
        enum ProxyType {
            PT_NONE = 0, 
            PT_HTTP, PT_HTTPS, PT_SOCKS4, PT_SOCKS5
        };

        // curl proxy
        class CurlProxy {
        protected:
            // type of proxy. could be http, https, socks4, socks5 or none
            ProxyType m_eType = PT_NONE;
            std::string m_sURL = "";
        public:
            CurlProxy();

            // get url
            virtual std::string getURL();

            // get proxy protocol
            virtual ProxyType getType();

            // generate proxy protocol out of the url
            static ProxyType fromString(std::string url);

            // constructor
            CurlProxy(std::string url);

            // destructor
            CurlProxy(const char * url);
        };

        // any parameters to url
        class CURLParameter {
        public:
            // constructor
            CURLParameter();
            // constructor
            CURLParameter(std::string k, std::string v);
            // constructor
            CURLParameter(std::string k, int v);
            
            std::string key;
            std::string value;
        };

        // data sent by cURL
        class CURLResult {
        public:
            // data size sent by curl
            int realSize;

            // result of the curl call
            int result;

            // max size of the data buffer
            int maxSize;

            // http status
            int http_status;

            // ratelimit value: how much client should wait for the next request
            int retry_after;
            
            // data
            const char *data;
        };

        // settings for cURL
        class CURLConnectionSettings {
        public:
            // write to the buffer
            bool m_bWriteToMemory;

            // file to write data
            FILE *m_pFile;

            // buffer to write data
            void *m_pData;

            // buffer size
            int m_nMaxMemorySize;

            // size
            int m_nSize;
            
            // run in debug mode
            bool sendDebug;

            CURLConnectionSettings(FILE *file);
            CURLConnectionSettings(void *memory_pointer, int maxSize);

            ~CURLConnectionSettings();
        };

        // implements cURL data fetching
        class CURLConnection {
        protected:
            // curl instance
            CURL *m_pCurl;

            // debug flag
            bool m_bDebug;

            // website parameters
            std::vector<CURLParameter*> m_mUserData;
            
            // proxy for curl
            CurlProxy m_proxy;

            // destroys curl instance and all parameters
            void destroy();
        public:
            // set debug fiag
            void setDebug(bool d);

            // set parameters
            void setData(std::vector<CURLParameter*> parameters);

            // add parameters
            void addData(std::vector<CURLParameter*> parameters);

            // set proxy for accessing pages
            void setProxy(CurlProxy proxy);

            virtual CURLResult *access_page(const char *url);
            virtual CURLResult *access_page(const char *url, FILE *file);
            virtual CURLResult *access_page(const char *url, const char *method);
            virtual CURLResult *access_page(const char *url, const char *method, FILE *file);
            
            // internal curl write function
            static size_t write_data(void *ptr, size_t size, size_t nmemb, void *stream);

            // constructor
            CURLConnection();
            // destructor
            ~CURLConnection();
        };
    }
}