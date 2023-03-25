#pragma once

#include "lapi_database.h"

namespace LevelAPI {
    namespace DatabaseController {
        void database_runner(Database *db);
        void node_runner(Node *nd);
        void node_runner_waitResolverRL(Node *nd, int rate_limit_length);
        void node_runner_recentBot(Node *nd);
    }
}