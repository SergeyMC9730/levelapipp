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
        // cmd1.add_option(
        //     dpp::command_option(
        //         dpp::co_string,
        //         "type",
        //         Translation::getByKey("lapi.bot.command.lapiget.option.type.info"),
        //         true
        //     ).add_choice(dpp::command_option_choice(
        //         Translation::getByKey("lapi.bot.command.lapiget.option.type.v1"),
        //         "by_id"
        //     )).add_choice(dpp::command_option_choice(
        //         Translation::getByKey("lapi.bot.command.lapiget.option.type.v2"),
        //         "by_user"
        //     )).add_choice(dpp::command_option_choice(
        //         Translation::getByKey("lapi.bot.command.lapiget.option.type.v3"),
        //         "by_name"
        //     )).add_choice(dpp::command_option_choice(
        //         Translation::getByKey("lapi.bot.command.lapiget.option.type.v4"),
        //         "by_description"
        //     ))
        // );
        // cmd1.add_option(
        //     dpp::command_option(
        //         dpp::co_string,
        //         "str",
        //         Translation::getByKey("lapi.bot.command.lapiget.option.str.info"),
        //         true
        //     )
        // );

        

        // auto grp1 = dpp::command_option(
        //     dpp::co_sub_command_group,
        //     "type",
        //     Translation::getByKey("lapi.bot.command.lapiget.option.type.info"),
        //     false
        // );
        // auto grp2 = dpp::command_option(
        //     dpp::co_sub_command_group,
        //     "filter",
        //     Translation::getByKey("lapi.bot.command.lapiget.option.filter.info"),
        //     false
        // );
        // grp1.add_option(
        //     dpp::command_option(
        //         dpp::co_sub_command,
        //         "levelid",
        //         Translation::getByKey("lapi.bot.command.lapiget.option.type.v1"),
        //         false
        //     ).add_option(
        //         dpp::command_option(
        //             dpp::co_integer,
        //             "levelid_field",
        //             Translation::getByKey("lapi.bot.command.lapiget.option.type.v1.field"),
        //             true
        //         )
        //     )
        // );
        // grp1.add_option(
        //     dpp::command_option(
        //         dpp::co_sub_command,
        //         "userid",
        //         Translation::getByKey("lapi.bot.command.lapiget.option.type.v2"),
        //         false
        //     ).add_option(
        //         dpp::command_option(
        //             dpp::co_integer,
        //             "userid_field",
        //             Translation::getByKey("lapi.bot.command.lapiget.option.type.v2.field"),
        //             true
        //         )
        //     )
        // );
        // grp1.add_option(
        //     dpp::command_option(
        //         dpp::co_sub_command,
        //         "accountid",
        //         Translation::getByKey("lapi.bot.command.lapiget.option.type.v3"),
        //         false
        //     ).add_option(
        //         dpp::command_option(
        //             dpp::co_integer,
        //             "accountid_field",
        //             Translation::getByKey("lapi.bot.command.lapiget.option.type.v3.field"),
        //             true
        //         )
        //     )
        // );
        // grp1.add_option(
        //     dpp::command_option(
        //         dpp::co_sub_command,
        //         "nickname",
        //         Translation::getByKey("lapi.bot.command.lapiget.option.type.v4"),
        //         false
        //     ).add_option(
        //         dpp::command_option(
        //             dpp::co_string,
        //             "nickname_field",
        //             Translation::getByKey("lapi.bot.command.lapiget.option.type.v4.field"),
        //             true
        //         )
        //     )
        // );
        // grp1.add_option(
        //     dpp::command_option(
        //         dpp::co_sub_command,
        //         "name",
        //         Translation::getByKey("lapi.bot.command.lapiget.option.type.v5"),
        //         false
        //     ).add_option(
        //         dpp::command_option(
        //             dpp::co_string,
        //             "name_field",
        //             Translation::getByKey("lapi.bot.command.lapiget.option.type.v5.field"),
        //             true
        //         )
        //     )
        // );
        // grp1.add_option(
        //     dpp::command_option(
        //         dpp::co_sub_command,
        //         "description",
        //         Translation::getByKey("lapi.bot.command.lapiget.option.type.v6"),
        //         false
        //     ).add_option(
        //         dpp::command_option(
        //             dpp::co_integer,
        //             "description_field",
        //             Translation::getByKey("lapi.bot.command.lapiget.option.type.v6.field"),
        //             true
        //         )
        //     )
        // );
        // grp2.add_option(
        //     dpp::command_option(
        //         dpp::co_sub_command,
        //         "stars",
        //         Translation::getByKey("lapi.bot.command.lapiget.option.filter.v1"),
        //         false
        //     ).add_option(
        //         dpp::command_option(
        //             dpp::co_integer,
        //             "stars_field",
        //             Translation::getByKey("lapi.bot.command.lapiget.option.filter.v1.field"),
        //             true
        //         )
        //     )
        // );

        auto co2 = dpp::command_option(
            dpp::co_string,
            "node",
            Translation::getByKey("lapi.bot.command.lapiget.option.node.info"),
            true
        );

        int i = 0;
        while(i < DatabaseController::database->m_vNodes.size()) {
            auto nd = DatabaseController::database->m_vNodes.at(i)->m_sInternalName;
            auto nd2 = nd;
            nd2[0] = std::toupper(nd2[0]);
            i++;
        }

        cmd1.add_option(
            dpp::command_option(
                dpp::co_string,
                "str",
                Translation::getByKey("lapi.bot.command.lapiget.option.str.info"),
                true
            )
        );
        cmd1.add_option(
            dpp::command_option(
                dpp::co_string,
                "sort",
                Translation::getByKey("lapi.bot.command.lapiget.option.sort.info"),
                false
            ).add_choice(
                dpp::command_option_choice(
                    Translation::getByKey("lapi.bot.command.lapiget.option.sort.v1"),
                    "mostliked"
                )
            ).add_choice(
                dpp::command_option_choice(
                    Translation::getByKey("lapi.bot.command.lapiget.option.sort.v2"),
                    "mostdownloaded"
                )
            )
        );
        cmd1.add_option(
            dpp::command_option(
                dpp::co_string,
                "searchfilter",
                Translation::getByKey("lapi.bot.command.lapiget.option.type.info"),
                false
            ).add_choice(
                dpp::command_option_choice(
                    Translation::getByKey("lapi.bot.command.lapiget.option.type.v1"),
                    "levelid"
                )
            ).add_choice(
                dpp::command_option_choice(
                    Translation::getByKey("lapi.bot.command.lapiget.option.type.v2"),
                    "userid"
                )
            ).add_choice(
                dpp::command_option_choice(
                    Translation::getByKey("lapi.bot.command.lapiget.option.type.v3"),
                    "accountid"
                )
            ).add_choice(
                dpp::command_option_choice(
                    Translation::getByKey("lapi.bot.command.lapiget.option.type.v4"),
                    "nickname"
                )
            ).add_choice(
                dpp::command_option_choice(
                    Translation::getByKey("lapi.bot.command.lapiget.option.type.v5"),
                    "name"
                )
            ).add_choice(
                dpp::command_option_choice(
                    Translation::getByKey("lapi.bot.command.lapiget.option.type.v6"),
                    "description"
                )
            )
        );
        cmd1.add_option(
            dpp::command_option(
                dpp::co_string,
                "sfstr",
                Translation::getByKey("lapi.bot.command.lapiget.option.type.field"),
                false
            )
        );
        // cmd1.add_option(
        //     grp1
        // );
        // cmd1.add_option(
        //     grp2
        // );
        
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
	        std::string animal = std::get<std::string>(event.get_parameter("str"));
	        event.reply(Translation::getByKey("lapi.bot.command.lapiget.output.test", animal));
	    }
	});
    instance->m_pBot->start(dpp::st_wait);
}