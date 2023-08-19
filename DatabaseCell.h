#pragma once

#include "SQLiteManager.h"
#include "json/single_include/nlohmann/json.hpp"

namespace LevelAPI {
    namespace DatabaseController {
        class DatabaseCell {
        public:
            // sqlite instance if we want to use it
            SQLiteManager *_sqliteObject = nullptr;
            // json data for saving and recovering the cell
            nlohmann::json _jsonObject = {};
        public:
            // destructor
            ~DatabaseCell();

            // save cell data into json
            virtual void save();
            // recover cell from json
            virtual void recover();

            // setup json data
            virtual void setupJSON();
            // create sqlite instance
            virtual void setupSQLite();
        };
    }
}
