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
#include "tgbot/types/GenericReply.h"
#include "tgbot/types/InlineKeyboardButton.h"
#include "tgbot/types/Message.h"
#include "tgbot/types/ReplyParameters.h"
#include "tgbot/types/User.h"
#include "TelegramInstance.hpp"
#include <optional>
#include <fmt/format.h>

using namespace LevelAPI;

Frontend::TCommand::TCommand(const std::string &name, const std::string &description) {
    _name = name;
    _description = description;
}

void Frontend::TCommand::run(TelegramInstance *instance, TgBot::Message::Ptr message) {
    _instance = instance;
    _pressedButton = {"", ""};

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

                ctx.user = message->from;
                ctx.user_message = message;
                ctx.bot_response = message->replyToMessage;

                run(type, &ctx);

                _userContext[ctx.user->id] = ctx;
                if (_deleteContext) {
                    clearContext(ctx.user);
                    _deleteContext = false;
                }
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
                createCustomData(&ctx);
                run(type, &ctx);

                _userContext[ctx.user->id] = ctx;
                if (_deleteContext) {
                    clearContext(ctx.user);
                    _deleteContext = false;
                }
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

            createCustomData(&ctx);
            run(type, &ctx);

            _userContext[ctx.user->id] = ctx;
            if (_deleteContext) {
                clearContext(ctx.user);
                _deleteContext = false;
            }
        }
    }
}

void Frontend::TCommand::run(TelegramInstance *instance, TgBot::CallbackQuery::Ptr query) {
    _instance = instance;

    printf("- user %ld clicked on a button\n", query->from->id);
    printf("- this button was %s\n", query->data.c_str());

    if (_tempFilter.count(query->data) && _userContext.count(query->from->id)) {
        printf("- we have this user in the memory\n");

        auto context = _userContext[query->from->id];

        context.user_message = query->message;
        context.bot_response = context.user_message;
        context.user = query->from;

        _pressedButton = {_tempFilter[query->data], query->data};

        run(CTButton, &context);

        _userContext[query->from->id] = context;
        if (_deleteContext) {
            clearContext(query->from);
            _deleteContext = false;
        }
    } else {
        printf("- we do not have this user. ignoring request\n");
    }
}

std::optional<struct Frontend::TCommand::UserContext> Frontend::TCommand::getContext(TgBot::User::Ptr user) const {
    if (!_userContext.count(user->id)) return std::nullopt;

    return _userContext.at(user->id);
}

void Frontend::TCommand::clearContext(TgBot::User::Ptr user) {
    if (_userContext.count(user->id)) {
        printf("- deleting user context for %ld\n", user->id);
        auto ctx = _userContext[user->id];
        deleteCustomData(&ctx);
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
            sendMessage(ctx, "Вы использовали неимплементированную команду.", {StrPair{"a", "b"}, {"c", "d"}, {"e", "f"}, {"g", "h"}});
            break;
        }
        case CTReply: {
            sendMessage(ctx, "Вы ответили на вызов неимплементированной команды.");
            break;
        }
        case CTButton: {
            StrPair pressed = getPressedButton();
            sendMessage(ctx, fmt::format("Вы кликнули на кнопку {} со значением {}", pressed.first, pressed.second));
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

void Frontend::TCommand::createCustomData(UserContext *ctx) {}
void Frontend::TCommand::deleteCustomData(UserContext *ctx) {}

TgBot::Message::Ptr Frontend::TCommand::sendMessage(UserContext *ctx, const std::string &msg, const std::vector<StrPair> &buttons) {
    TgBot::ReplyParameters::Ptr reply(new TgBot::ReplyParameters);

    reply->messageId = ctx->user_message->messageId;
    reply->chatId = ctx->user_message->chat->id;

    TgBot::InlineKeyboardMarkup::Ptr keyboard = nullptr;
    if (buttons.size() > 0) {
        keyboard = TgBot::InlineKeyboardMarkup::Ptr(new TgBot::InlineKeyboardMarkup);

        size_t buttons_per_row = 3;
        size_t row_amount = std::ceil((float)buttons.size() / (float)buttons_per_row);

        size_t offset = 0;
        for (size_t y = 0; y < row_amount; y++) {
            std::vector<TgBot::InlineKeyboardButton::Ptr> row = {};
            for (size_t x = 0; x < buttons_per_row; x++) {
                if (offset + x >= buttons.size()) break;

                auto bdata = buttons[offset + x];

                TgBot::InlineKeyboardButton::Ptr button(new TgBot::InlineKeyboardButton);
                button->text = bdata.first;
                button->callbackData = bdata.second;

                _tempFilter[bdata.second] = bdata.first;

                row.push_back(button);
            }

            offset += buttons_per_row;

            if (!row.empty()) {
                keyboard->inlineKeyboard.push_back(row);
            }
        }
    }

    ctx->bot_response = _instance->m_pBot->getApi().sendMessage(ctx->user_message->chat->id, msg, nullptr, reply, keyboard, "HTML");
    return ctx->bot_response;
}

TgBot::Message::Ptr Frontend::TCommand::sendMessage(UserContext *ctx, const std::string &msg) {
    std::vector<std::string> b = {};
    return sendMessage(ctx, msg, b);
}

TgBot::Message::Ptr Frontend::TCommand::sendMessage(UserContext *ctx, const std::string &msg, const std::string &button) {
    return sendMessage(ctx, msg, {button});
}

TgBot::Message::Ptr Frontend::TCommand::sendMessage(UserContext *ctx, const std::string &msg, const std::vector<std::string> &buttons)  {
    std::vector<StrPair> pairs = {};

    for (const std::string &b : buttons) {
        pairs.push_back({b, b});
    }

    return sendMessage(ctx, msg, pairs);
}

Frontend::TCommand::StrPair Frontend::TCommand::getPressedButton() {
    return _pressedButton;
}

void Frontend::TCommand::requestContextDeletion() {
    _deleteContext = true;
}
