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

#include "DiscordInstance.h"
#include "DCommand.h"
#include "appcommand.h"
#include "cluster.h"
#include "dispatcher.h"
#include "lapi_database.h"
#include "Translation.h"
#include "presence.h"
#include "iAndy.h"

#include "DCommandStats.h"
#include "DCommandSearch.h"

using namespace std::chrono_literals;
using namespace LevelAPI::Frontend;

DiscordInstance::DiscordInstance(LevelAPI::DatabaseController::Database *db) {
    this->m_pDB = db;
    m_pBot = new dpp::cluster(db->discord.m_sBotToken, dpp::i_default_intents | dpp::i_message_content);
    m_pBot->on_log(dpp::utility::cout_logger());

}

std::thread *DiscordInstance::start() {
    std::thread *dst = new std::thread(DiscordInstance::dthread, this);
    dst->detach();
    return dst;
}

void DiscordInstance::setStatus(std::string status) {
    dpp::presence_status st = dpp::ps_dnd;
    auto pr = dpp::presence(st, dpp::activity_type::at_listening, status);
    auto bot = m_pBot;

    bot->set_presence(pr);
}

void DiscordInstance::dthread(DiscordInstance *instance) {
    auto dbA = reinterpret_cast<LevelAPI::DatabaseController::Database *>(instance->m_pDB);
    auto bot = instance->m_pBot;

    std::vector<DCommand *> commandList = {};

    bot->on_ready([&](const dpp::ready_t& event) {
        std::cout << Translation::getByKey("lapi.bot.command.create") << std::endl;

        commandList.push_back(new DCommandStats(instance->m_pBot->me.id));
        commandList.push_back(new DCommandSearch(instance->m_pBot->me.id));

        int i = 0;
        while (i < commandList.size()) {
            std::cout << Translation::getByKey("lapi.bot.command.register", commandList[i]->getCommandName()) << std::endl;

            auto cmd = commandList[i]->getCommand();

            bot->global_command_create(cmd);

            i++;
        }

        dbA->discord.m_bBotReady = true;
        std::cout << Translation::getByKey("lapi.bot.ready") << std::endl;
	});

    instance->m_pBot->on_message_create([&](const dpp::message_create_t& event) {
        if(!event.msg.author.is_bot() && !event.msg.is_dm()) {
            if(event.msg.content == "lapi:registerchannel") {
                if(dbA->discord.m_sRegisteredCID.empty()) {
                    event.reply(Translation::getByKey("lapi.bot.command.registerchannel.success"));
                    uint64_t val = event.msg.channel_id;
                    dbA->discord.m_sRegisteredCID = std::to_string(val);
                    dbA->save();
                }
            }
        }
	});
    bot->on_slashcommand([&](const dpp::slashcommand_t & event) {
        int i = 0;
        while (i < commandList.size()) {
            if (event.command.get_command_name() == commandList[i]->getCommandName()) {
                std::cout << Translation::getByKey("lapi.bot.command.run", commandList[i]->getCommandName()) << std::endl;
                commandList[i]->_cluster = bot;
                commandList[i]->run(event);
            }

            i++;
        }
	});
    bot->on_button_click([&](const dpp::button_click_t & event) {
        int i = 0;

        while (i < commandList.size()) {
            auto event_list = commandList[i]->getRegistedEvents();

            int j = 0;
            while (j < event_list.size()) {
                if (event.custom_id == event_list[j].first) {
                    commandList[i]->_cluster = bot;
                    commandList[i]->run(event);
                }

                j++;
            }

            i++;
        }
	});
    bot->on_select_click([&](const dpp::select_click_t & event) {
	    int i = 0;

        while (i < commandList.size()) {
            auto event_list = commandList[i]->getRegistedEvents();

            int j = 0;
            while (j < event_list.size()) {
                if (event.custom_id == event_list[j].first) {
                    commandList[i]->_cluster = bot;
                    commandList[i]->run(event);
                }

                j++;
            }

            i++;
        }
	});

    instance->m_pBot->start(dpp::st_return);

    while(true) {
        int ll = dbA->getTotalLevels();
        float scaleMult = 1.f;
        std::string levels = iAndy::intToFormatString(ll, scaleMult);
        std::string levelssofar = Translation::getByKey("lapi.bot.status.levels", levels);
        instance->setStatus(levelssofar);
        std::this_thread::sleep_for(std::chrono::seconds(30s));
    }
}
