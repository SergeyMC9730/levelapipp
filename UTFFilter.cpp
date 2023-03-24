#include "Tools.h"

using namespace std;

// https://stackoverflow.com/questions/17316506/strip-invalid-utf8-from-string-in-c-c

void LevelAPI::Tools::filterString(std::string *data) {
    if(data->size() == 0) return;
    data->resize(data->size() - 2);
}