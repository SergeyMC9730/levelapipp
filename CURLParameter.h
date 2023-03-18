#pragma once

#include <string>

class CURLParameter {
public:
    CURLParameter();
    CURLParameter(const char *k, const char *v);

    ~CURLParameter();

    const char *key;
    const char *value;
};
