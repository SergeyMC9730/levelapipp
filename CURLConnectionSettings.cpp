#include "curl_backend.h"

using namespace LevelAPI::Backend;

CURLConnectionSettings::CURLConnectionSettings(FILE *file) {
    m_bWriteToMemory = false;
    m_pFile = file;
    m_nSize = 0;
}
CURLConnectionSettings::CURLConnectionSettings(void *memory_pointer, int maxSize) {
    m_bWriteToMemory = true;
    m_nMaxMemorySize = maxSize;
    m_pData = memory_pointer;
    m_nSize = 0;
}
CURLConnectionSettings::~CURLConnectionSettings() {}