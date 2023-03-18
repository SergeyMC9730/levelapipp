#include "CURLParameter.h"
#include <string>

CURLParameter::CURLParameter() {
    CURLParameter("", "");
}
CURLParameter::CURLParameter(const char * k, const char * v) {
    key = k;
    value = v;
}
CURLParameter::~CURLParameter() {
    printf("~curlparameter\n");
    return;
}