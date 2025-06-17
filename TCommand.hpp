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

#include "tgbot/types/CallbackQuery.h"
#include "tgbot/types/Message.h"
#include <tgbot/tgbot.h>
#include <map>
#include <optional>

namespace LevelAPI {
    namespace Frontend {
        class TelegramInstance;

        class UserContextExt {
        public:
            int filler;
        };

        class TCommand {
        protected:
            class StrPair {
            public:
                std::string first = "";
                std::string second = "";

                StrPair() {}
                StrPair(const std::string &_first, const std::string &_second) {first = _first; second = _second;}
            };
        private:
            StrPair _pressedButton;
            std::map<std::string, std::string> _tempFilter;

            bool _deleteContext = false;
        protected:
            enum CommandType {
                CTCommandCall,
                CTReply,
                CTButton
            };
            struct UserContext {
                TgBot::User::Ptr user;
                TgBot::Message::Ptr user_message;
                TgBot::Message::Ptr bot_response;
                void *data = nullptr;
            };

            std::map<int64_t, UserContext> _userContext;

            std::string _name;
            std::string _description;

            TelegramInstance *_instance;

            virtual void run(enum CommandType type, UserContext *ctx);

            virtual void createCustomData(UserContext *ctx);
            virtual void deleteCustomData(UserContext *ctx);

            void requestContextDeletion();

            virtual TgBot::Message::Ptr sendMessage(UserContext *ctx, const std::string &msg, const std::vector<StrPair> &buttons);
            TgBot::Message::Ptr sendMessage(UserContext *ctx, const std::string &msg, const std::vector<std::string> &buttons);
            TgBot::Message::Ptr sendMessage(UserContext *ctx, const std::string &msg, const std::string &button);
            TgBot::Message::Ptr sendMessage(UserContext *ctx, const std::string &msg);

            StrPair getPressedButton();
        public:
            TCommand(const std::string &name, const std::string &description);

            operator TgBot::BotCommand::Ptr() const;
            TgBot::BotCommand::Ptr build() const;

            void run(TelegramInstance *instance, TgBot::Message::Ptr message);
            void run(TelegramInstance *instance, TgBot::CallbackQuery::Ptr query);

            std::optional<struct UserContext> getContext(TgBot::User::Ptr user) const;
            void clearContext(TgBot::User::Ptr user);
        };
    }
}
