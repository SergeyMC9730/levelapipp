#include "CURLParameter.h"
#include <string>

CURLParameter::CURLParameter() {
    key = "";
    value = "";
}
CURLParameter::CURLParameter(const char * k, const char * v) {
    key = k;
    value = v;
}
CURLParameter::~CURLParameter() {
    return;
}