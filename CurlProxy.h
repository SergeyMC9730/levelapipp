#pragma once

#include <string>

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

    CurlProxy(std::string url);
    CurlProxy(const char * url);
};