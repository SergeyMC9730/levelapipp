#include "Tools.h"

void LevelAPI::Tools::filterString(std::string *data) {
    if(data->size() == 0) return;
    data->resize(data->size() - 2);
}