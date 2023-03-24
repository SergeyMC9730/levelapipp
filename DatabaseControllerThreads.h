#pragma once

#include "lapi_database.h"

namespace LevelAPI {
    namespace DatabaseController {
        void database_runner(Database *db);
        void node_runner(Node *nd);
    }
}