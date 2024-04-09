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

#include "cluster.h"
#include <dpp/dpp.h>

#include <thread>

namespace LevelAPI {
    namespace Frontend {
        class DiscordInstance {
        public:
            // linked cluster
            dpp::cluster *m_pBot;

            // SQLiteManager class pointer
            void *m_pDB;

            DiscordInstance(void *db);

	    // thread for this DiscordInstance
            static void dthread(DiscordInstance *instance); 

	    // set bot status (offline, dnd, afk, online)
            void setStatus(std::string status);

	    // run thread and return it
            std::thread *start();
        };
    }
}
