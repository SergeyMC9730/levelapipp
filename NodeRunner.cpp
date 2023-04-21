#include "DatabaseControllerThreads.h"

#include <chrono>
#include <string>
#include <thread>
#include <iostream>
#include <vector>

#include "GDServer.h"
#include "Tools.h"
#include "GDServer_BoomlingsLike21.h"
#include "GDServer_BoomlingsLike19.h"
#include "lapi_database.h"
#include "message.h"
#include "restresults.h"

#include "Translation.h"

using namespace LevelAPI;
using namespace LevelAPI::Frontend;
using namespace std::chrono_literals;

void DatabaseController::node_runner_recount_task(Node *nd) {
    std::string folder = "database/nodes/" + std::string(nd->m_sInternalName->c_str()) + "/levels";
    nd->m_vCachedLevels.clear();
    for (const auto & entry : std::filesystem::directory_iterator(folder)) {
        std::string path = entry.path();
        std::string filename = path.substr(path.find_last_of("/\\") + 1);
        std::string levelid = filename.substr(filename.find_last_of("_") + 1);
        nd->m_vCachedLevels.push_back(std::stoi(levelid));
    }
    if(!nd->m_pPolicy->m_bNoOutput) {
        std::cout << Translation::getByKey("lapi.noderunner.recount.complete", *nd->m_sInternalName, nd->m_vCachedLevels.size()) << std::endl;
    }
    std::this_thread::sleep_for(10s);
}

void DatabaseController::node_runner_recentBot(Node *nd) {
    if(!nd->m_pPolicy->m_bEnableRecentTab) return;
start:
    std::this_thread::sleep_for(std::chrono::milliseconds((int)(nd->m_pPolicy->m_nQueueProcessingInterval * 1000)));
    if(nd->m_uQueue->m_vCommandQueue->size() == 0) {
        nd->m_uQueue->m_vCommandQueue->push_back(new NodeCommandQueue(NC_RECENT, new std::string("-")));
        nd->m_uQueue->save();
    }
    goto start;
}

void DatabaseController::node_runner_waitResolverRL(Node *nd, int rate_limit_length) {
    if(nd->m_bRateLimitApplied) return;
    
    nd->m_bRateLimitApplied = true;
    
    if(!nd->m_pPolicy->m_bNoOutput) {
        //std::cout << "[LevelAPI resolver " << *nd->m_sInternalName << "] Waiting " << rate_limit_length << "s" << std::endl;
        std::cout << Translation::getByKey("lapi.noderunner.resolver.wait.start", *nd->m_sInternalName, rate_limit_length) << std::endl;
    }
    std::this_thread::sleep_for(std::chrono::seconds(rate_limit_length + (int)nd->m_pPolicy->m_nResolverInterval));
    if(!nd->m_pPolicy->m_bNoOutput) {
        //std::cout << "[LevelAPI resolver " << *nd->m_sInternalName << "] Rate limit end!" << std::endl;
        std::cout << Translation::getByKey("lapi.noderunner.resolver.wait.end", *nd->m_sInternalName) << std::endl;
    }

    nd->m_bRateLimitApplied = false;
    return;
}

void DatabaseController::node_runner_wait_level(Node *nd, dpp::message message, int id) {
    return;
    if(nd->m_bRateLimitApplied) return;
    nd->m_uQueue->m_vCommandQueue->push_back(new NodeCommandQueue(NC_ID, new std::string(std::to_string(id))));
    while(1) {
        if(nd->m_bRateLimitApplied) return;
        auto level = nd->getLevel(id);
        if(!nd->m_pPolicy->m_bNoOutput) {
            //std::cout << "[LevelAPI resolver " << *nd->m_sInternalName << "] Waiting for " << id << " " << level->m_bHasLevelString << std::endl;
            std::cout << Translation::getByKey("lapi.curltest.lvlwait.start", *nd->m_sInternalName, id, level->m_bHasLevelString) << std::endl;
        }
        if(level->m_bHasLevelString) {
            auto level = nd->getLevel(id);
            message.embeds.clear();
            message.add_embed(level->getAsEmbed());
            DatabaseController::database->m_pLinkedBot->m_pBot->message_edit(
                message
            );
            return;
        }
        // delete level;
        std::this_thread::sleep_for(500ms);
    }
}

void DatabaseController::node_runner_resolve_level(Node *nd, NodeCommandQueue *q, Backend::GDServer *server) {
    nd->m_bResolverIsActive = true;

    int id = std::stoi(std::string(q->m_sText->c_str()));
    float time_to_wait = nd->m_pPolicy->m_nResolverInterval - nd->m_pPolicy->m_nQueueProcessingInterval;
    if(time_to_wait < 0.f) time_to_wait = 0;
    if(!nd->m_pPolicy->m_bNoOutput) {
        std::cout << Translation::getByKey("lapi.noderunner.downloader.sleeping.fetch", *nd->m_sInternalName, time_to_wait, id) << std::endl;
    }
            
    std::this_thread::sleep_for(
        std::chrono::milliseconds(
            (int)(time_to_wait * 1000.f)
        )
    );

    if(nd->m_bRateLimitApplied && nd->m_pPolicy->m_bWaitResolverRL) {
        if(!nd->m_pPolicy->m_bNoOutput) {
            std::cout << Translation::getByKey("lapi.noderunner.resolver.error.ratelimit", *nd->m_sInternalName) << std::endl;
        }
        return;
    }
    if(!nd->m_pPolicy->m_bEnableResolver) {
        if(!nd->m_pPolicy->m_bNoOutput) {
            std::cout << Translation::getByKey("lapi.noderunner.resolver.error.disabled", *nd->m_sInternalName) << std::endl;
        }
        return;
    }

    auto level = server->getLevelMetaByID(id, false);
    if(level->m_nRetryAfter != 0) {
        if(!nd->m_pPolicy->m_bWaitResolverRL || level->m_nRetryAfter < 0) return;
        nd->m_nWaitTime = level->m_nRetryAfter;
        if(!nd->m_pPolicy->m_bNoOutput) {
            std::cout << Translation::getByKey("lapi.noderunner.resolver.event.ratelimit", *nd->m_sInternalName, nd->m_nWaitTime) << std::endl;
        }
        nd->m_bRateLimitApplied = true;
        delete level;
        level = nullptr;
    } else {
        nd->initLevel(level);
        level->m_bHasLevelString = true;
        level->save();
        nd->m_jLastDownloadedLevel = level->levelJson;
        if(!nd->m_pPolicy->m_bNoOutput) {
            std::cout << Translation::getByKey("lapi.noderunner.downloader.event.levelfetched", *nd->m_sInternalName, id) << std::endl;
        }
        delete level;
        level = nullptr;
    }

    delete q;
    q = nullptr;

    nd->m_bResolverIsActive = false;
}

void DatabaseController::node_runner(Node *nd) {
    std::cout << Translation::getByKey("lapi.noderunner.start", *nd->m_sInternalName) << std::endl;

    std::vector<int> recent_downloadedids;

    Backend::GDServer *server;
    switch(nd->m_uDatabase->m_nFeatureSet) {
        default:
        case 21: {
            server = static_cast<Backend::GDServer *>(new Backend::GDServer_BoomlingsLike21(nd->m_uDatabase->m_sEndpoint));
            break;
        }
        case 19: {
            server = static_cast<Backend::GDServer *>(new Backend::GDServer_BoomlingsLike19(nd->m_uDatabase->m_sEndpoint));
            break;
        }
    }

    server->setDebug(false);

    std::thread rcbt(DatabaseController::node_runner_recentBot, nd);
    rcbt.detach();

    std::thread rtt(DatabaseController::node_runner_recount_task, nd);
    rtt.detach();

    Level *llevel;

    goto start_linear;

error_ignore:
    if(!nd->m_pPolicy->m_bNoOutput) {
        std::cout << Translation::getByKey("lapi.noderunner.downloader.error.queue.empty", *nd->m_sInternalName) << std::endl;
    }
    goto run_again;
loop_readonly2:
    if(!nd->m_pPolicy->m_bNoOutput) {
        std::cout << Translation::getByKey("lapi.noderunner.readonly", *nd->m_sInternalName) << std::endl;
    }
    //delete server;
loop_readonly:
    if(!nd->m_pPolicy->m_bNoOutput) {
        std::cout << Translation::getByKey("lapi.noderunner.readonly", *nd->m_sInternalName) << std::endl;
    }
    while(true) {
        std::this_thread::sleep_for(std::chrono::seconds(2s));
    }
run_again:
    if(nd->m_bRateLimitApplied) {
        std::thread rlt(DatabaseController::node_runner_waitResolverRL, nd, nd->m_nWaitTime);
        rlt.detach();
    }
start_linear:
    if (nd->m_uDatabase->m_bReadOnly) goto loop_readonly2;

    if(!nd->m_pPolicy->m_bEnableLinearResolver) goto start;
    nd->m_uQueue->m_bExecuteQueue = !nd->m_uQueue->m_bExecuteQueue;
    nd->m_uQueue->save();
    if(nd->m_uQueue->m_bExecuteQueue || nd->m_bRateLimitApplied) goto start;
    else {
        if(nd->m_pPolicy->m_bEnableResolver) {
            if(!nd->m_pPolicy->m_bNoOutput) {
                std::cout << Translation::getByKey("lapi.noderunner.resolver.linear.wait", *nd->m_sInternalName, nd->m_pPolicy->m_nResolverInterval) << std::endl;
            }

            std::this_thread::sleep_for(
                std::chrono::milliseconds(
                    (int)(nd->m_pPolicy->m_nResolverInterval * 1000.f)
                )
            );

            if(!nd->m_pPolicy->m_bNoOutput) {
                std::cout << Translation::getByKey("lapi.noderunner.resolver.linear.fetch", *nd->m_sInternalName, nd->m_uQueue->m_nRuntimeState) << std::endl;
            }

            llevel = server->getLevelMetaByID(nd->m_uQueue->m_nRuntimeState, false);

            if (llevel->m_nRetryAfter == 0) {
                nd->initLevel(llevel);
                llevel->m_bHasLevelString = true;
                llevel->save();
                nd->m_jLastDownloadedLevel = llevel->levelJson;
                delete llevel;
                llevel = nullptr;
                //if(!nd->m_pPolicy->m_bNoOutput) std::cout << "[LevelAPI linear resolver " << *nd->m_sInternalName << "] Fetched level " << nd->m_uQueue->m_nRuntimeState << std::endl;
                if(!nd->m_pPolicy->m_bNoOutput) {
                    std::cout << Translation::getByKey("lapi.noderunner.resolver.linear.event.fetch.end", *nd->m_sInternalName, nd->m_uQueue->m_nRuntimeState) << std::endl;
                }
            } else if (llevel->m_nRetryAfter > 0 && nd->m_pPolicy->m_bWaitResolverRL) {
                nd->m_nWaitTime = llevel->m_nRetryAfter;
                //if(!nd->m_pPolicy->m_bNoOutput) std::cout << "[LevelAPI linear resolver " << *nd->m_sInternalName << "] RATE LIMIT for " << nd->m_nWaitTime << "s" << std::endl;
                if(!nd->m_pPolicy->m_bNoOutput) {
                    std::cout << Translation::getByKey("lapi.noderunner.resolver.linear.event.ratelimit", *nd->m_sInternalName, nd->m_nWaitTime) << std::endl;
                }
                nd->m_bRateLimitApplied = true;
                delete llevel;
                llevel = nullptr;
            }

            nd->m_uQueue->m_nRuntimeState++;   
        } else {
            if(!nd->m_pPolicy->m_bNoOutput) {
                std::cout << Translation::getByKey("lapi.noderunner.resolver.linear.error.disabled", *nd->m_sInternalName) << std::endl;
            }
        }
        goto run_again;
    }
start:
    if(!nd->m_pPolicy->m_bEnableLinearResolver) {
        //if(!nd->m_pPolicy->m_bNoOutput) std::cout << "[LevelAPI downloader " << *nd->m_sInternalName << "] Waiting " << nd->m_pPolicy->m_nQueueProcessingInterval << "s" << std::endl;
        if(!nd->m_pPolicy->m_bNoOutput) {
            std::cout << Translation::getByKey("lapi.noderunner.downloader.wait", *nd->m_sInternalName, nd->m_pPolicy->m_nQueueProcessingInterval) << std::endl;
        }

        std::this_thread::sleep_for(
            std::chrono::milliseconds((int)(nd->m_pPolicy->m_nQueueProcessingInterval * 1000.f))
        );
    }

    if(nd->m_uQueue->m_vCommandQueue->size() == 0) goto error_ignore;

    auto q = nd->m_uQueue->m_vCommandQueue->at(0);

    nd->m_uQueue->currentState = q->m_nCommand;

    switch(q->m_nCommand) {
        case NC_RECENT: {
            if(!nd->m_pPolicy->m_bEnableRecentTab) break;

            if(!nd->m_pPolicy->m_bNoOutput) {
                std::cout << Translation::getByKey("lapi.noderunner.downloader.recenttab.fetch", *nd->m_sInternalName) << std::endl;
            }
            auto levels = server->getLevelsBySearchType(4);
            int i = 0;

            std::vector<int> new_levels;

            while(i < levels.size()) {
                int levelid;
                std::string levelname;
                levelid = levels[i]->m_nLevelID;
                levelname = std::string(levels[i]->m_sLevelName->c_str());
                    
                levels[i]->m_sLinkedNode = std::string(nd->m_sInternalName->c_str());
                nd->initLevel(levels[i]);
                levels[i]->m_bHasLevelString = false;
                levels[i]->save();
                nd->m_jLastDownloadedLevel = levels[i]->levelJson;

                if(!std::count(recent_downloadedids.begin(), recent_downloadedids.end(), levels[i]->m_nLevelID)) {
                    new_levels.push_back(levelid);
                    recent_downloadedids.push_back(levelid);
                    if (!DatabaseController::database->m_sRegisteredCID.empty() && DatabaseController::database->m_bBotReady) {
                        DatabaseController::database->m_pLinkedBot->m_pBot->message_create(dpp::message(
                            dpp::snowflake(DatabaseController::database->m_sRegisteredCID), levels[i]->getAsEmbed()
                        ), [&](const dpp::confirmation_callback_t &ct) {
                            if(!ct.is_error() && !nd->m_bRateLimitApplied && nd->m_pPolicy->m_bWaitResolverRL && nd->m_pPolicy->m_bEnableResolver) {
                                std::thread wlt(DatabaseController::node_runner_wait_level, nd, std::get<dpp::message>(ct.value), levelid);
                                wlt.detach();
                            }
                        });
                    }
                }

                delete levels[i];
                levels[i] = nullptr;

                // if(!nd->m_pPolicy->m_bNoOutput) std::cout << "[LevelAPI downloader " << *nd->m_sInternalName << "] Level saved without level string: " << levelid << " \"" << levelname << "\"" << std::endl; 
                if(!nd->m_pPolicy->m_bNoOutput) {
                    std::cout << Translation::getByKey("lapi.noderunner.downloader.event.complete.noresolver", *nd->m_sInternalName, levelid, levelname) << std::endl;
                }
                i++;
            }
            levels.clear();

            break;
        }
        case NC_ID: {
            if(!nd->m_bResolverIsActive) {
                std::thread rlt(DatabaseController::node_runner_resolve_level, nd, q, server);
                rlt.detach();
            }

            break;
        }
        default:
        case NC_NONE:
        case NC_IDLE: break;
    }
    if(q->m_nCommand != NC_ID) {
        delete q;
        q = nullptr;
    }
    nd->m_uQueue->m_vCommandQueue->erase(nd->m_uQueue->m_vCommandQueue->begin());
    nd->m_uQueue->save();

    nd->m_uQueue->currentState = NC_NONE;

    goto run_again;
}
