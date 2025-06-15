/**
 *  LevelAPI - Geometry Dash level cacher with search functionality and more.
    Copyright (C) 2025  Sergei Baigerov

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

#include "tgbot/Bot.h"
#include <tgbot/tgbot.h>

namespace LevelAPI {
    namespace DatabaseController {
        class Database;
    }
    namespace Frontend {
        class TelegramInstance {
        private:
            std::thread m_tTgThread;
            bool m_bThreadStarted = false;
        public:
            // linked bot
            TgBot::Bot *m_pBot;

            // levelapi instance
            DatabaseController::Database *m_pDB = nullptr;

            TelegramInstance(DatabaseController::Database *db);

            void start();
            void close();

            // thread for this TelegramInstance
            void thread();
        };
    }
}
