#include "DCommandStats.h"

#include "DCommand.h"

#include "HttpController.h"
#include "SearchFilter.h"
#include "appcommand.h"
#include "format.h"
#include "lapi_database.h"
#include "message.h"

#include "Translation.h"

using namespace LevelAPI;
using namespace LevelAPI::Frontend;

DCommand::DCommandCallback DCommandStats::getDefaultCallback() {
    return [&](DCommand *command, DCommandEvent *event) {
        // event->_event.thinking(true);

        // std::cout << event->getEvent()->raw_event << std::endl;

        auto node = event->getArgument<std::string>("node");
        // auto node = std::get<std::string>(event->getEvent()->get_parameter("animal"));
        // event->getEvent()->

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

            event->_event.reply(msg);

            return;
        }

        dpp::embed emb;

        emb.set_title(Translation::getByKey("lapi.command.stats.execute.success.title"));
        emb.set_color(0x40df8d);
        emb.set_footer(dpp::embed_footer().set_text("LevelAPI  •  " + node));
        emb.set_timestamp(time(0));
        emb.add_field(
            Translation::getByKey("lapi.command.stats.execute.success.field.levels"),
            fmt::format("**{}**", node_object->m_nCachedLevels),
            true
        );
        emb.add_field(
            Translation::getByKey("lapi.command.stats.execute.success.field.qjobs"),
            fmt::format("**{}**", node_object->m_uQueue->m_vCommandQueue.size()),
            true
        );
        emb.add_field(
            Translation::getByKey("lapi.command.stats.execute.success.field.qdjobs"),
            fmt::format("**{}**", node_object->m_uQueue->m_vResolverQueuedLevels.size()),
            true
        );

        auto filter = new SearchFilter();

        filter->m_nLevelsPerPage = 5;
        filter->m_nPage = 1;

        filter->m_eSort = SearchSort::SSLatestDBApperead;
        
        auto levels = node_object->getLevels(filter);

        delete filter;
        filter = nullptr;

        dpp::component component_latest;
        dpp::component actionrow1;
        dpp::component actionrow2;

        actionrow1.set_type(dpp::cot_action_row);
        actionrow2.set_type(dpp::cot_action_row);

        component_latest.set_type(dpp::cot_button);
        component_latest.set_emoji("✅");
        component_latest.set_label(Translation::getByKey("lapi.command.stats.execute.success.ldl"));
        component_latest.set_style(dpp::cos_success);
        component_latest.set_disabled(true);
        component_latest.set_id("latest-levels");

        actionrow1.add_component(component_latest);

        int i = 0;
        while (i < levels.size()) {
            dpp::component component_level;

            std::string url = HttpController::getURL();
            std::string download_url = fmt::format("{}/api/v1/level/download?id={}&node={}", url, levels[i]->m_nLevelID, node);

            component_level.set_type(dpp::cot_button);
            component_level.set_emoji("▶");
            component_level.set_label(levels[i]->m_sLevelName);
            component_level.set_style(dpp::cos_link);
            component_level.set_id(fmt::format("level-{}_{}-brief", node, levels[i]->m_nLevelID));
            component_level.set_url(download_url);

            actionrow2.add_component(component_level);

            delete levels[i];
            levels[i] = nullptr;

            i++;
        }

        dpp::message msg;

        msg.add_embed(emb);
        msg.add_component(actionrow1);
        msg.add_component(actionrow2);

        event->_event.reply(msg);

        return;
    };
}

std::string DCommandStats::getCommandName() {
    return "stats";
}

DCommandStats::DCommandStats(dpp::snowflake botID) : DCommand(botID, getDefaultCallback()) {
    dpp::slashcommand cmd(getCommandName(), Translation::getByKey("lapi.command.stats.description"), botID);
    int i = 0;

    dpp::command_option opt1(dpp::co_string, "node", Translation::getByKey("lapi.command.stats.node.description"), true);

    while (i < DatabaseController::database->m_vNodes.size()) {
        auto node = DatabaseController::database->m_vNodes.at(i);

        dpp::command_option_choice choice(node->m_sInternalName, node->m_sInternalName);

        opt1.add_choice(choice);

        i++;
    }

    cmd.add_option(opt1);

    _instance = cmd;

    return;
}

DCommandStats::~DCommandStats() {}