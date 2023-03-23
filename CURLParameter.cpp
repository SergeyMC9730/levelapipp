#include "CURLParameter.h"
#include <cstdio>
#include <string>
#include <stdlib.h>
#include <string.h>

CURLParameter::CURLParameter() {
    key = "";
    value = "";

    eraseBuffers();
}
CURLParameter::CURLParameter(const char * k, const char * v) {
    key = k;
    value = v;

    eraseBuffers();
}
CURLParameter::CURLParameter(const char * k, int v) {
    key = k;

    eraseBuffers();
    
    buffer0 = (char *)malloc(16);
    sprintf(buffer0, "%d", v);
    
    value = buffer0;
}
CURLParameter::CURLParameter(std::string k, std::string v) {
    eraseBuffers();

    buffer0 = (char *)malloc(k.size() + 1);
    buffer1 = (char *)malloc(v.size() + 1);

    memcpy(buffer0, k.c_str(), k.size());
    memcpy(buffer1, v.c_str(), v.size());

    key = buffer0;
    value = buffer1;
}
CURLParameter::CURLParameter(std::string k, int v) {
    eraseBuffers();

    buffer0 = (char *)malloc(k.size() + 1);
    buffer1 = (char *)malloc(16);

    memcpy(buffer0, k.c_str(), k.size());
    sprintf(buffer1, "%d", v);

    key = buffer0;
    value = buffer1;
}
CURLParameter::~CURLParameter() {
    deallocBuffers();
    eraseBuffers();
    return;
}

void CURLParameter::eraseBuffers() {
    buffer0 = nullptr;
    buffer1 = nullptr;
    buffer2 = nullptr;
}
void CURLParameter::deallocBuffers() {
    if(buffer0) free(buffer0);
    if(buffer1) free(buffer1);
    if(buffer2) free(buffer2);
}