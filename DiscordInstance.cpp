#include "DiscordInstance.h"
#include "appcommand.h"
#include "cluster.h"
#include "lapi_database.h"
#include "Translation.h"
#include "presence.h"
#include "iAndy.h"

using namespace LevelAPI::DiscordController;
using namespace std::chrono_literals;
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

void DiscordInstance::setStatus(std::string status) {
    dpp::presence_status st = dpp::ps_dnd;
    auto pr = dpp::presence(st, dpp::activity_type::at_listening, status);
    auto bot = m_pBot;
    
    bot->set_presence(pr);
}

void DiscordInstance::dthread(DiscordInstance *instance) {
    auto dbA = reinterpret_cast<LevelAPI::DatabaseController::Database *>(instance->m_pDB);
    auto bot = instance->m_pBot;
    bot->on_ready([&](const dpp::ready_t& event) {
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
	    // if (event.command.get_command_name() == "lapiget") {
	    //     std::string animal = std::get<std::string>(event.get_parameter("str"));
	    //     event.reply(Translation::getByKey("lapi.bot.command.lapiget.output.test", animal));
	    // }
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