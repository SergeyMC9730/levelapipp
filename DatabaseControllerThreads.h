#pragma once

#include "GDServer.h"
#include "lapi_database.h"
#include "message.h"

namespace LevelAPI {
    namespace DatabaseController {
        void database_runner(Database *db);
        void node_runner(Node *nd);
        void node_runner_waitResolverRL(Node *nd, int rate_limit_length);
        void node_runner_recentBot(Node *nd);
        void node_runner_recount_controller();
        void node_runner_recount_task();
        void node_runner_wait_level(Node *nd, dpp::message message, int id);
        void node_runner_resolve_level(Node *nd, NodeCommandQueue *q, Backend::GDServer *server);
    }
}