#pragma once

#include <string>

class CURLParameter {
private:
    char *buffer0;
    char *buffer1;
    char *buffer2;

    void eraseBuffers();
    void deallocBuffers();
public:
    CURLParameter();
    CURLParameter(const char *k, const char *v);
    CURLParameter(const char *k, int v);
    CURLParameter(std::string k, std::string v);
    CURLParameter(std::string k, int v);

    ~CURLParameter();

    const char *key;
    const char *value;
};
