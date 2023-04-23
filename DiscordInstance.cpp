#include "DiscordInstance.h"
#include "appcommand.h"
#include "cluster.h"
#include "lapi_database.h"
#include "Translation.h"

using namespace LevelAPI::DiscordController;
using namespace LevelAPI::Frontend;

DiscordInstance::DiscordInstance(void *db) {
    this->m_pDB = db;
    auto dbA = reinterpret_cast<LevelAPI::DatabaseController::Database *>(db);
    m_pBot = new dpp::cluster(dbA->m_sBotToken, dpp::i_default_intents | dpp::i_message_content);
    m_pBot->on_log(dpp::utility::cout_logger());
}

std::thread *DiscordInstance::start() {
    std::thread *dst = new std::thread(DiscordInstance::dthread, this);
    dst->detach();
    return dst;
}

void DiscordInstance::dthread(DiscordInstance *instance) {
    auto dbA = reinterpret_cast<LevelAPI::DatabaseController::Database *>(instance->m_pDB);
    auto bot = instance->m_pBot;
    bot->on_ready([&](const dpp::ready_t& event) {
        auto cmd1 = dpp::slashcommand("lapiget", Translation::getByKey("lapi.bot.command.lapiget.info.base"), bot->me.id);
        cmd1.add_option(
            dpp::command_option(
                dpp::co_string,
                "type",
                Translation::getByKey("lapi.bot.command.lapiget.option.type.info"),
                true
            ).add_choice(dpp::command_option_choice(
                Translation::getByKey("lapi.bot.command.lapiget.option.type.v1"),
                "by_id"
            )).add_choice(dpp::command_option_choice(
                Translation::getByKey("lapi.bot.command.lapiget.option.type.v2"),
                "by_user"
            )).add_choice(dpp::command_option_choice(
                Translation::getByKey("lapi.bot.command.lapiget.option.type.v3"),
                "by_name"
            )).add_choice(dpp::command_option_choice(
                Translation::getByKey("lapi.bot.command.lapiget.option.type.v4"),
                "by_description"
            ))
        );
        cmd1.add_option(
            dpp::command_option(
                dpp::co_string,
                "str",
                Translation::getByKey("lapi.bot.command.lapiget.option.str.info"),
                true
            )
        );
        bot->global_command_create( 
	        cmd1
	    );
	    dbA->m_bBotReady = true;
        std::cout << Translation::getByKey("lapi.bot.ready") << std::endl;
	});
    instance->m_pBot->on_message_create([&](const dpp::message_create_t& event) {
        if(!event.msg.author.is_bot() && !event.msg.is_dm()) {
            if(event.msg.content == "lapi:registerchannel") {
                if(dbA->m_sRegisteredCID.empty()) {
                    event.reply(Translation::getByKey("lapi.bot.command.registerchannel.success"));
                    uint64_t val = event.msg.channel_id;
                    dbA->m_sRegisteredCID = std::to_string(val);
                    dbA->save();  
                } 
            }
        }
	});
    bot->on_slashcommand([&bot](const dpp::slashcommand_t & event) {
	    if (event.command.get_command_name() == "lapiget") {
	        std::string animal = std::get<std::string>(event.get_parameter("type"));
	        event.reply(Translation::getByKey("lapi.bot.command.lapiget.output.test", animal));
	    }
	});
    instance->m_pBot->start(dpp::st_wait);
}