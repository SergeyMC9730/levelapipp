#include "CurlProxy.h"

CurlProxy::CurlProxy() {
    m_sURL = "";
    m_eType = PT_NONE;
}

CurlProxy::CurlProxy(std::string url) {
    m_sURL = url;

    // https://curl.se/libcurl/c/CURLOPT_PROXY.html

    if (url.find("http://") != std::string::npos) {
        m_eType = PT_HTTP;
    } else if (url.find("https://") != std::string::npos) {
        m_eType = PT_HTTPS;
    } else if (url.find("socks4://") != std::string::npos) {
        m_eType = PT_SOCKS4;
    } else if (url.find("socks5://") != std::string::npos) {
        m_eType = PT_SOCKS5;
    } else {
        m_eType = PT_NONE;
    }
}

std::string CurlProxy::getURL() {
    return m_sURL;
}
ProxyType CurlProxy::getType() {
    return m_eType;
}