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

#include "TCommand.hpp"
#include "tgbot/tools/StringTools.h"
#include "tgbot/types/User.h"
#include "TelegramInstance.hpp"
#include <optional>

using namespace LevelAPI;

Frontend::TCommand::TCommand(const std::string &name, const std::string &description) {
    _name = name;
    _description = description;
}

void Frontend::TCommand::run(TelegramInstance *instance, TgBot::Message::Ptr message) {
    _instance = instance;

    TCommand::CommandType type = CTCommandCall;

    if (_userContext.count(message->from->id)) {
        printf("- user exists in the context of this command (uid=%ld)\n",
            message->from->id
        );

        UserContext ctx = _userContext[message->from->id];
        if (message->replyToMessage) {
            printf("- user replyed to \"%s\" with id %d\n",
                message->replyToMessage->text.c_str(),
                message->replyToMessage->messageId
            );
            if (ctx.bot_response) {
                printf("- we have to compare replied %d with what bot already sent (%d of \"%s\")\n",
                    message->replyToMessage->messageId,
                    ctx.bot_response->messageId,
                    ctx.bot_response->text.c_str()
                );
                if (ctx.bot_response->messageId == message->replyToMessage->messageId) {
                    printf("- it's a reply to sent from our bot message\n");
                    type = CTReply;
                }

                UserContext ctx;

                ctx.user = message->from;
                ctx.user_message = message;
                ctx.bot_response = message->replyToMessage;

                run(type, &ctx);

                _userContext[ctx.user->id] = ctx;
            }
        } else {
            printf("- user did not reply to message\n");

            std::string c = "/" + _name;
            UserContext ctx;

            ctx.user = message->from;
            ctx.user_message = message;
            ctx.bot_response = nullptr;

            if (StringTools::startsWith(message->text, c)) {
                printf("- message starts with %s (full msg is %s) so user definitely sent a command call\n",
                    c.c_str(),
                    message->text.c_str()
                );

                clearContext(ctx.user);
                run(type, &ctx);

                _userContext[ctx.user->id] = ctx;
            }
        }
    } else {
        printf("- user does not exist in this command so he's new\n");

        std::string c = "/" + _name;
        UserContext ctx;

        ctx.user = message->from;
        ctx.user_message = message;
        ctx.bot_response = nullptr;

        if (StringTools::startsWith(message->text, c)) {
            printf("- message starts with %s (full msg is %s) so user definitely sent a command call\n",
                c.c_str(),
                message->text.c_str()
            );

            run(type, &ctx);

            _userContext[ctx.user->id] = ctx;
        }
    }
}

std::optional<struct Frontend::TCommand::UserContext> Frontend::TCommand::getContext(TgBot::User::Ptr user) const {
    if (!_userContext.count(user->id)) return std::nullopt;

    return _userContext.at(user->id);
}

void Frontend::TCommand::clearContext(TgBot::User::Ptr user) {
    if (_userContext.count(user->id)) {
        _userContext.erase(user->id);
    }
}

void Frontend::TCommand::run(enum CommandType type, UserContext *ctx) {
    printf("command ran with type %d; username is %s; orig message content is %s\n",
        (int)type,
        ctx->user->username.c_str(),
        ctx->user_message->text.c_str()
    );

    switch (type) {
        case CTCommandCall: {
            ctx->bot_response = _instance->m_pBot->getApi().sendMessage(ctx->user_message->chat->id, "Вы использовали неимплеменитированную команду.");
            break;
        }
        case CTReply: {
            ctx->bot_response = _instance->m_pBot->getApi().sendMessage(ctx->user_message->chat->id, "Вы ответили на вызов неимплеменитированной команды.");
            break;
        }
    }
}

Frontend::TCommand::operator TgBot::BotCommand::Ptr() const {
    return build();
}

TgBot::BotCommand::Ptr Frontend::TCommand::build() const {
    TgBot::BotCommand::Ptr cmdArray(new TgBot::BotCommand);

    cmdArray->command = _name;
    cmdArray->description = _description;

    return cmdArray;
}
