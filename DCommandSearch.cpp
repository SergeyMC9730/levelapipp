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

#include "DCommandSearch.h"
#include "Level.h"
#include "SearchFilter.h"
#include "appcommand.h"
#include "dispatcher.h"
#include "format.h"
#include "message.h"
#include "lapi_database.h"

#include "Translation.h"
#include "restresults.h"

#include <map>

using namespace LevelAPI;
using namespace LevelAPI::Frontend;
using namespace LevelAPI::Backend;

SearchFilter *SearchInstance::getParent() {
    return reinterpret_cast<SearchFilter *>(getSelf());
}
SearchInstance *SearchInstance::getSelf() {
    return this;
}

dpp::message DCommandSearch::render(std::vector<LevelAPI::DatabaseController::Level *> levels, std::string node, int page) {
    dpp::message msg;

    if (levels.size() == 2 && levels[1] == nullptr) {
        if (levels[0] != nullptr) {
            levels[0]->m_sLinkedNode = node;

            if (levels[0]->m_sUsername.empty()) {
                levels[0]->m_sUsername = "-";
            }

            msg.add_embed(levels[0]->getAsEmbed(LevelAPI::DatabaseController::LevelAppearanceEvent::E_INFORMATION));
        
            dpp::component actionrow1;
            actionrow1.set_type(dpp::cot_action_row);

            dpp::component btn_morelevels;
            btn_morelevels.set_type(dpp::cot_button);
            btn_morelevels.set_emoji("👤");
            btn_morelevels.set_id("btn_morelevels_player");
            btn_morelevels.set_style(dpp::cos_success);
            btn_morelevels.set_label(Translation::getByKey("lapi.command.search.morelevels"));

            actionrow1.add_component(btn_morelevels);

            msg.add_component(actionrow1);
        } else {
            dpp::embed emb;
            emb.set_title(Translation::getByKey("lapi.command.search.id.error.title"));
            emb.set_description(Translation::getByKey("lapi.command.search.id.error.description"));
            emb.set_color(0xff0000);
            emb.set_footer(dpp::embed_footer().set_text("LevelAPI  •  " + node));
            emb.set_timestamp(time(0));
            msg.add_embed(emb);
        }
    } else {
        std::string embed_text = "";
        if (page != -1) {
            embed_text = Translation::getByKey("lapi.command.search.page", page);
        }

        dpp::component actionrow1;
        dpp::component actionrow2;

        actionrow1.set_type(dpp::cot_action_row);
        actionrow2.set_type(dpp::cot_action_row);

        dpp::component btn_prev;
        dpp::component btn_next;

        btn_prev.set_type(dpp::cot_button);
        btn_prev.set_emoji("⬅️");
        btn_prev.set_id("btn_prevpage");
        btn_prev.set_style(dpp::cos_secondary);
        btn_prev.set_label(Translation::getByKey("lapi.command.search.button.prevpage"));
        btn_prev.set_disabled(page <= 1 || levels.size() < 1);

        btn_next.set_type(dpp::cot_button);
        btn_next.set_emoji("➡️");
        btn_next.set_id("btn_nextpage");
        btn_next.set_style(dpp::cos_secondary);
        btn_next.set_label(Translation::getByKey("lapi.command.search.button.nextpage"));
        btn_next.set_disabled(levels.size() < 5);

        actionrow1.add_component(btn_prev);
        actionrow1.add_component(btn_next);

        dpp::component select_men;
        select_men.set_type(dpp::cot_selectmenu);
        select_men.set_placeholder(Translation::getByKey("lapi.command.search.selectmenu.levels"));
        select_men.set_id("select_level");

        int i = 0;
        while (i < levels.size()) {
            if (levels[i]->m_sUsername.empty()) {
                levels[i]->m_sUsername = "-";
            }

            embed_text += Translation::getByKey("lapi.command.search.list.level", levels[i]->m_sLevelName, levels[i]->m_sUsername, levels[i]->m_nLevelID);

            dpp::select_option opt(levels[i]->m_sLevelName, std::to_string(levels[i]->m_nLevelID), fmt::format("Uploaded by {}", levels[i]->m_sUsername));

            select_men.add_select_option(opt);

            i++;
        }

        dpp::embed emb;
        emb.set_title(Translation::getByKey("lapi.command.search.list.title"));
        emb.set_description(embed_text);
        emb.set_color(0x48ff00);
        emb.set_footer(dpp::embed_footer().set_text("LevelAPI  •  " + node));
        emb.set_timestamp(time(0));

        actionrow2.add_component(select_men);

        msg.add_embed(emb);
        msg.add_component(actionrow1);
        if (levels.size() != 0) msg.add_component(actionrow2);
    }

    return msg;
}

DCommand::DCommandCallback DCommandSearch::getDefaultCallback() {
    return [&](DCommand *command, DCommandEvent *event) {
        auto command_search = dynamic_cast<DCommandSearch *>(command);

        auto node = event->getArgument<std::string>("node");
    
        auto node_object = DatabaseController::database->getNode(node);

        if (!node_object) {
            nlohmann::json j_emb;

            dpp::embed emb;

            emb.set_color(0xCB1515);
            emb.set_title(Translation::getByKey("lapi.command.stats.execute.error.title"));
            emb.set_description(Translation::getByKey("lapi.command.stats.execute.error.description"));
            emb.set_footer(dpp::embed_footer().set_text("LevelAPI  •  " + node));
            emb.set_timestamp(time(0));

            dpp::message msg;

            msg.add_embed(emb);
            msg.set_flags(dpp::message_flags::m_ephemeral);

            event->getCommandEvent().reply(msg);

            return;
        }

        auto userid = event->getCommandEvent().command.member.user_id;

        command_search->_instances[userid] = SearchInstance();
        command_search->_instances[userid]._discordUserID = userid;
        command_search->_instances[userid]._node = node;
        command_search->_instances[userid]._discordCluster = command->_cluster;

        if (event->argumentExists("id")) {
            command_search->_instances[userid]._discordType = SearchInstanceType::SearchID;

            int levelid = event->getArgument<int>("id");

            command_search->_instances[userid]._levelID = levelid;

            auto level = node_object->getLevel(levelid);

            auto msg = command_search->render({level, nullptr}, node, 1);
        
            if (level != nullptr) {
                command_search->_instances[userid].m_nUID = level->m_nPlayerID;
            
                delete level;
                level = nullptr;
            }

            auto ev_ = event->getCommandEvent();

            ev_.reply(msg, [command_search, userid, ev_](const dpp::confirmation_callback_t &ev) {
                if (!ev.is_error()) {
                    ev_.get_original_response([command_search, userid](const dpp::confirmation_callback_t &ev) {
                        if (!ev.is_error()) {
                            auto msg = std::get<dpp::message>(ev.value);
                            command_search->_instances[userid]._discordMessage = msg;
                        }

                    });
                }
            });

            return;
        }

        command_search->_instances[userid]._discordType = SearchInstanceType::SearchList;

        std::map<std::string, SearchSort> sortmap = {
            {"most-liked", SSMostLiked},
            {"most-downloaded", SSMostDownloaded},
            {"last-appeared", SSLatestDBApperead},
            {"recent", SSRecentLevel},
            {"oldest", SSOldestLevel},
            {"none", SSNone}
        };

        #define SET_VARIABLE(name, mname, type) if (event->argumentExists(name)) mname = event->getArgument<type>(name);

        SET_VARIABLE("difficulty", command_search->_instances[userid].m_nDifficulty, int);
        SET_VARIABLE("stars", command_search->_instances[userid].m_nStars, int);
        SET_VARIABLE("name", command_search->_instances[userid].m_sName, std::string);
        SET_VARIABLE("description", command_search->_instances[userid].m_sDescription, std::string);
        command_search->_instances[userid].m_nPage = 1;
        SET_VARIABLE("username", command_search->_instances[userid].m_sUsername, std::string);
        
        if (command_search->_instances[userid].m_nDifficulty == 60) {
            command_search->_instances[userid].m_nDifficulty = -1;
            command_search->_instances[userid].m_nStars = 10;
        }

        std::string sorting = "";
        SET_VARIABLE("sort", sorting, std::string);
        if (sortmap.count(sorting) > 0) {
            command_search->_instances[userid].m_eSort = sortmap[sorting];
        }

        command_search->_instances[userid].m_nLevelsPerPage = 5;

        auto levels = node_object->getLevels(*(command_search->_instances[userid].getParent()));

        if (!command_search->_instances[userid].m_sUsername.empty() && levels.size() != 0) {
            command_search->_instances[userid].m_nUID = levels[0]->m_nPlayerID;
        }

        auto msg = command_search->render(levels, node, 1);

        int i = 0;
        while (i < levels.size()) {
            delete levels[i];
            levels[i] = nullptr;

            i++;
        }

        auto ev_ = event->getCommandEvent();
        
        ev_.reply(msg, [command_search, userid, ev_](const dpp::confirmation_callback_t &ev) {
            if (!ev.is_error()) {
                ev_.get_original_response([command_search, userid](const dpp::confirmation_callback_t &ev) {
                    if (!ev.is_error()) {
                        auto msg = std::get<dpp::message>(ev.value);
                        command_search->_instances[userid]._discordMessage = msg;
                    }

                });
            }
        });
    };
}

void DCommandSearch::eventCallback(std::string name, DCommandEvent *event) {
    std::vector<DatabaseController::Level *> _levels = {};

    if (name == "btn_morelevels_player") {
        if (_instances.count(event->getButtonEvent().command.member.user_id)) {
            auto context = _instances[event->getButtonEvent().command.member.user_id].getSelf();

            context->m_nPage = 1;
            context->m_sName = "";
            context->m_sUsername = "";
            context->m_nDifficulty = -1;
            context->m_nStars = -1;
            context->m_eSort = SSRecentLevel;
            context->m_sDescription = "";
            context->_discordType = SearchInstanceType::SearchList;

            auto node_object = DatabaseController::database->getNode(context->_node);

            _levels = node_object->getLevels(*context);

            auto msg = render(_levels, context->_node, context->m_nPage);

            context->_discordMessage.embeds = msg.embeds;
            context->_discordMessage.components = msg.components;
            context->_discordMessage.content = msg.content;

            context->_discordCluster->message_edit(context->_discordMessage);
        }
    }

    if (name == "btn_prevpage") {
        if (_instances.count(event->getButtonEvent().command.member.user_id)) {
            auto context = _instances[event->getButtonEvent().command.member.user_id].getSelf();

            if (context->m_nPage > 1) context->m_nPage--;

            auto node_object = DatabaseController::database->getNode(context->_node);

            _levels = node_object->getLevels(*context);

            auto msg = render(_levels, context->_node, context->m_nPage);

            context->_discordMessage.embeds = msg.embeds;
            context->_discordMessage.components = msg.components;
            context->_discordMessage.content = msg.content;

            context->_discordCluster->message_edit(context->_discordMessage);
        }
    }

    if (name == "btn_nextpage") {
        if (_instances.count(event->getButtonEvent().command.member.user_id)) {
            auto context = _instances[event->getButtonEvent().command.member.user_id].getSelf();

            context->m_nPage++;

            auto node_object = DatabaseController::database->getNode(context->_node);

            _levels = node_object->getLevels(*context);

            auto msg = render(_levels, context->_node, context->m_nPage);

            context->_discordMessage.embeds = msg.embeds;
            context->_discordMessage.components = msg.components;
            context->_discordMessage.content = msg.content;

            context->_discordCluster->message_edit(context->_discordMessage);
        }
    }

    if (name == "select_level") {
        if (_instances.count(event->getSelectMenuEvent().command.member.user_id)) {
            auto context = _instances[event->getSelectMenuEvent().command.member.user_id].getSelf();

            int id = std::stoi(event->getSelectMenuEvent().values[0]);

            context->m_nPage = 1;
            context->m_sName = "";
            context->m_sUsername = "";
            context->m_nDifficulty = -1;
            context->m_nStars = -1;
            context->m_eSort = SSNone;
            context->m_sDescription = "";
            context->_discordType = SearchInstanceType::SearchID;
            context->_levelID = id;

            auto node_object = DatabaseController::database->getNode(context->_node);

            auto level = node_object->getLevel(id);

            auto msg = render({level, nullptr}, context->_node, context->m_nPage);

            if (level != nullptr) {
                context->m_nUID = level->m_nPlayerID;

                delete level;
                level = nullptr;
            }

            context->_discordMessage.embeds = msg.embeds;
            context->_discordMessage.components = msg.components;
            context->_discordMessage.content = msg.content;

            context->_discordCluster->message_edit(context->_discordMessage);
        }
    }

    int i = 0;
    while (i < _levels.size()) {
        delete _levels[i];
        _levels[i] = nullptr;

        i++;
    }
}

DCommandSearch::DCommandSearch(dpp::snowflake botID) : DCommand(botID, getDefaultCallback()) {
    dpp::slashcommand cmd(getCommandName(), Translation::getByKey("lapi.command.search.description"), botID);

    int i = 0;

    dpp::command_option opt1(dpp::co_string, "node", Translation::getByKey("lapi.command.stats.node.description"), true);
    dpp::command_option opt2(dpp::co_integer, "difficulty", Translation::getByKey("lapi.command.search.difficulty"), false);
    dpp::command_option opt3(dpp::co_integer, "stars", Translation::getByKey("lapi.command.search.stars"), false);
    dpp::command_option opt4(dpp::co_string, "name", Translation::getByKey("lapi.command.search.name"), false);
    dpp::command_option opt5(dpp::co_string, "description", Translation::getByKey("lapi.command.search.leveldescription"), false);
    dpp::command_option opt6(dpp::co_string, "username", Translation::getByKey("lapi.command.search.username"), false);
    dpp::command_option opt7(dpp::co_integer, "id", Translation::getByKey("lapi.command.search.id"), false);
    dpp::command_option opt8(dpp::co_string, "sort", Translation::getByKey("lapi.command.search.sort"), false);

    std::map<std::string, std::string> sortmap = {
        {"most-liked", Translation::getByKey("lapi.command.search.sort.most-liked")},
        {"most-downloaded", Translation::getByKey("lapi.command.search.sort.most-downloaded")},
        {"last-appeared", Translation::getByKey("lapi.command.search.sort.last-appeared")},
        {"recent", Translation::getByKey("lapi.command.search.sort.recent")},
        {"oldest", Translation::getByKey("lapi.command.search.sort.oldest")},
        {"none", Translation::getByKey("lapi.command.search.sort.none")}
    };

    opt2.add_choice({
        Translation::getByKey("lapi.command.search.difficulty.na"), 0
    });
    // opt2.add_choice({
    //     Translation::getByKey("lapi.command.search.difficulty.auto"), -3
    // });
    opt2.add_choice({
        Translation::getByKey("lapi.command.search.difficulty.easy"), 10
    });
    opt2.add_choice({
        Translation::getByKey("lapi.command.search.difficulty.normal"), 20
    });
    opt2.add_choice({
        Translation::getByKey("lapi.command.search.difficulty.hard"), 30
    });
    opt2.add_choice({
        Translation::getByKey("lapi.command.search.difficulty.harder"), 40
    });
    opt2.add_choice({
        Translation::getByKey("lapi.command.search.difficulty.insane"), 50
    });
    opt2.add_choice({
        Translation::getByKey("lapi.command.search.difficulty.demon"), 60
    });
    // opt2.add_choice({
    //     Translation::getByKey("lapi.command.search.difficulty.edemon"), 61
    // });
    // opt2.add_choice({
    //     Translation::getByKey("lapi.command.search.difficulty.mdemon"), 62
    // });
    // opt2.add_choice({
    //     Translation::getByKey("lapi.command.search.difficulty.hdemon"), 63
    // });
    // opt2.add_choice({
    //     Translation::getByKey("lapi.command.search.difficulty.idemon"), 64
    // });
    // opt2.add_choice({
    //     Translation::getByKey("lapi.command.search.difficulty.exdemon"), 65
    // });

    opt3.set_min_value(0);
    opt3.set_max_value(10);

    for (auto [key, val] : sortmap) {
        opt8.add_choice({
            val, key
        });
    }

    while (i < DatabaseController::database->m_vNodes.size()) {
        auto node = DatabaseController::database->m_vNodes.at(i);

        dpp::command_option_choice choice(node->m_sInternalName, node->m_sInternalName);

        opt1.add_choice(choice);

        i++;
    }

    cmd.add_option(opt1);
    cmd.add_option(opt7);
    cmd.add_option(opt2);
    cmd.add_option(opt3);
    cmd.add_option(opt4);
    cmd.add_option(opt5);
    cmd.add_option(opt6);
    cmd.add_option(opt8);

    registerEvent("btn_morelevels_player", DCommandEventType::ET_Button);
    registerEvent("btn_prevpage", DCommandEventType::ET_Button);
    registerEvent("btn_nextpage", DCommandEventType::ET_Button);
    registerEvent("select_level", DCommandEventType::ET_SelectMenu);

    _instance = cmd;
}
DCommandSearch::~DCommandSearch() {}

std::string DCommandSearch::getCommandName() {
    return "search";
};