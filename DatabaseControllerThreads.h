/**
 *  LevelAPI - Geometry Dash level cacher with search functionality and more.
    Copyright (C) 2023  Sergei Baigerov

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <https://www.gnu.org/licenses/>.
*/

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
        void node_runner_recount_task(Node *nds);
        void node_runner_wait_level(Node *nd, dpp::message message, int id);
        void node_runner_resolve_level(Node *nd, NodeCommandQueue *q, Backend::GDServer *server);
    }
}