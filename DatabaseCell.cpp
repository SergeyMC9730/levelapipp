#include "lapi_database.h"

using namespace LevelAPI::DatabaseController;

void DatabaseCell::save() {}
void DatabaseCell::recover() {}

void DatabaseCell::setupJSON() {}
void DatabaseCell::setupSQLite() {}

DatabaseCell::~DatabaseCell() {
    if (_sqliteObject) {
        delete _sqliteObject;
        
        _sqliteObject = nullptr;
    }
}