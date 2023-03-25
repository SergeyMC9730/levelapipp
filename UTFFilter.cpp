#include "Tools.h"

using namespace std;

void LevelAPI::Tools::filterString(std::string *data) {
    if(data->size() == 0) return;
    data->resize(data->size() - 2);
}