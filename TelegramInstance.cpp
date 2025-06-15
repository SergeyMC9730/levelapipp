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

#include "TelegramInstance.hpp"
#include "TCommand.hpp"
#include "lapi_database.h"
#include "tgbot/Bot.h"
#include "tgbot/TgException.h"
#include "tgbot/net/TgLongPoll.h"
#include "tgbot/types/MessageId.h"
#include <fmt/format.h>

using namespace LevelAPI;

Frontend::TelegramInstance::TelegramInstance(DatabaseController::Database *db) {
    m_pDB = db;
    if (!m_pDB) return;
}

void Frontend::TelegramInstance::start() {
    printf("starting telegram bot\n");

    if (m_bThreadStarted) {
        close();
    }

    m_bThreadStarted = true;
    m_tTgThread = std::thread([this]() {
       this->thread();
    });
}

void Frontend::TelegramInstance::thread() {
    TgBot::Bot bot(m_pDB->telegram.m_sBotToken);
    m_pBot = &bot;

    printf("Bot username: %s\n", bot.getApi().getMe()->username.c_str());

    std::vector<TCommand *> commands = {
        new TCommand("lstat", "get levelapi stats")
    };

    std::vector<TgBot::BotCommand::Ptr> builtCommands;

    for (TCommand *cmd : commands) {
        builtCommands.push_back(cmd->build());
    }

    bot.getApi().setMyCommands(builtCommands);

    std::vector<TgBot::BotCommand::Ptr> vectCmd;
    vectCmd = bot.getApi().getMyCommands();

    for (auto it = vectCmd.begin(); it != vectCmd.end(); ++it) {
        printf("cmd: %s -> %s\n",(*it)->command.c_str(),(*it)->description.c_str());
    }

    bot.getEvents().onAnyMessage([&bot, this, commands](TgBot::Message::Ptr message) {
        for (TCommand *cmd : commands) {
            cmd->run(this, message);
        }
    });

    TgBot::TgLongPoll poll(bot, 100, 100);
    bot.getApi().deleteWebhook();

    while (m_bThreadStarted) {
        printf("Long poll started\n");
        try {
            poll.start();
        } catch (TgBot::TgException &e) {
            printf("error: %s\n", e.what());
        }
    }
}

void Frontend::TelegramInstance::close() {
    if (!m_bThreadStarted) return;

    printf("closing telegram bot\n");

    m_bThreadStarted = false;
    if (!m_pBot->getApi().close()) {
        printf("tgbot close failed\n");
    }

    m_tTgThread.join();
}
