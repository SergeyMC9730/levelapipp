#include "CURLParameter.h"
#include <string>

CURLParameter::CURLParameter() {
    key = "";
    value = "";
}
CURLParameter::CURLParameter(std::string k, std::string v) {
    key = k;
    value = v;
}
CURLParameter::CURLParameter(std::string k, int v) {
    key = k;
    value = std::to_string(v);
}