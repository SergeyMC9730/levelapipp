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

#include <tgbot/tgbot.h>
#include <map>
#include <optional>

namespace LevelAPI {
    namespace Frontend {
        class TelegramInstance;

        class TCommand {
        protected:
            enum CommandType {
                CTCommandCall,
                CTReply
            };
            struct UserContext {
                TgBot::User::Ptr user;
                TgBot::Message::Ptr user_message;
                TgBot::Message::Ptr bot_response;
            };
            std::map<int64_t, UserContext> _userContext;

            std::string _name;
            std::string _description;

            TelegramInstance *_instance;

            virtual void run(enum CommandType type, UserContext *ctx);
        public:
            TCommand(const std::string &name, const std::string &description);

            operator TgBot::BotCommand::Ptr() const;
            TgBot::BotCommand::Ptr build() const;

            void run(TelegramInstance *instance, TgBot::Message::Ptr message);

            std::optional<struct UserContext> getContext(TgBot::User::Ptr user) const;
            void clearContext(TgBot::User::Ptr user);
        };
    }
}
