#pragma once

#include "SQLiteManager.h"
#include "json/single_include/nlohmann/json.hpp"

namespace LevelAPI {
    namespace DatabaseController {
        class DatabaseCell {
        public:
            SQLiteManager *_sqliteObject = nullptr;
            nlohmann::json _jsonObject = {};
        public:
            ~DatabaseCell();

            virtual void save();
            virtual void recover();
            
            virtual void setupJSON();
            virtual void setupSQLite();
        };
    }
}