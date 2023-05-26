#include "DatabaseControllerThreads.h"

#include "json/single_include/nlohmann/json.hpp"
#include <chrono>
#include <fstream>
#include <stdexcept>
#include <string>
#include <thread>
#include <iostream>
#include <vector>
#include <queue>

#include "GDServer.h"
#include "Level.h"
#include "lapi_database.h"
#include "message.h"
#include "restresults.h"

#include "Translation.h"

using namespace LevelAPI;
using namespace LevelAPI::Frontend;
using namespace std::chrono_literals;

void DatabaseController::node_runner_recount_task(Node *nd) {
    goto lp;
    lp:
    std::string folder = "database/nodes/" + nd->m_sInternalName + "/levels";
    nd->m_vCachedLevels.clear();
    for (const auto & entry : std::filesystem::directory_iterator(folder)) {
        std::string path = entry.path();
        std::string filename = path.substr(path.find_last_of("/\\") + 1);
        //std::cout << filename << std::endl;
        std::string levelid = filename.substr(filename.find_last_of("_") + 1);
        try {
            nd->m_vCachedLevels.push_back(std::stoi(levelid));
        } catch (std::invalid_argument &e) {}
    }
    if(!nd->m_pPolicy->m_bNoOutput) {
        std::cout << Translation::getByKey("lapi.noderunner.recount.complete", nd->m_sInternalName, nd->m_vCachedLevels.size()) << std::endl;
    }
    nd->m_nCachedLevels = nd->m_vCachedLevels.size();

    auto start = std::chrono::high_resolution_clock::now();

    int i = 0;
    int i2 = 0;

    std::vector<std::thread *> job_threads;
    std::vector<std::queue<int>> jobs_per_thread;
    std::vector<int> thread_pointers;
    std::vector<int> levels_completed;
    std::vector<int> thread_states;

    std::vector<int> history;
    nlohmann::json jsonreport;

    std::thread t1 = std::thread([&]{
        g:
        int i3 = 0;
        int lvls = 0;
	int lvls1 = 0;
        while(i3 < job_threads.size()) {
            lvls += levels_completed[i3];
            levels_completed[i3] = 0;
            i3++;
        }
	i3 = 0;
	history.push_back(lvls);
	while(i3 < history.size()) {
		lvls1 += history[i3];
		i3++;
	}
        jsonreport["report"] = history;
        std::ofstream rep("report.json");
        rep << jsonreport.dump();
        rep.close();
        std::cout << lvls << " levels per second" << std::endl;
	std::cout << lvls1 << " levels so far" << std::endl;
        std::this_thread::sleep_for(1s);
        goto g;
    });
    t1.detach();


    auto simple_job = [&](int id) {
        loop:
        if(jobs_per_thread[id].size() != 0) {
            int lid = jobs_per_thread[id].front();
            thread_states[id] = 1;
            //std::cout << "[" << id << "] Parsing " << lid << std::endl;;
            auto level1 = nd->getLevel(lid);
            delete level1;
            levels_completed[id]++;
            jobs_per_thread[id].pop();
        } else {
            //std::cout << "[" << id << "] Waiting " << std::endl;;
            thread_states[id] = 0;
        }
        //std::this_thread::sleep_for(10ms);

        goto loop;
    };

    while(i < std::thread::hardware_concurrency()) {
        jobs_per_thread.push_back(std::queue<int>());
        thread_pointers.push_back(i);
        levels_completed.push_back(0);
        thread_states.push_back(0);
        job_threads.push_back(new std::thread(simple_job, i));
        i++;
    }

    i = 0;
    int p = 0;

    while(i < nd->m_nCachedLevels) {
        if(thread_states[p] == 0) {
            jobs_per_thread[p].push(nd->m_vCachedLevels[i]);
            p++;
            p = p % job_threads.size();
            i++;
        } else {
            p++;
            p = p % job_threads.size();
        }
    }

    i = 0;
    bool ended = false;
    lp1:
    int j = 0;
    while(j < job_threads.size()) {
        if(jobs_per_thread[j].size() != 0) {
            ended = false;
            j = 0;
            goto lp;
        } else {
            ended = true;
        }
        j++;
        std::this_thread::sleep_for(100ms);
    }
    

    auto stop = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(stop - start);
 
    std::cout << "Time taken by function: " << duration.count() << " ms" << std::endl;

    std::this_thread::sleep_for(5s);
    goto lp;
}

void DatabaseController::node_runner_recentBot(Node *nd) {
    if(!nd->m_pPolicy->m_bEnableRecentTab) return;
start:
    std::this_thread::sleep_for(std::chrono::milliseconds((int)(nd->m_pPolicy->m_nQueueProcessingInterval * 1000)));
    if(nd->m_uQueue->m_vCommandQueue.size() == 0) {
        nd->m_uQueue->m_vCommandQueue.push_back(new NodeCommandQueue(NC_RECENT, "-"));
        nd->m_uQueue->save();
    }
    goto start;
}

void DatabaseController::node_runner_waitResolverRL(Node *nd, int rate_limit_length) {
    if(nd->m_bRateLimitApplied) return;
    
    nd->m_bRateLimitApplied = true;
    
    if(!nd->m_pPolicy->m_bNoOutput) {
        //std::cout << "[LevelAPI resolver " << nd->m_sInternalName << "] Waiting " << rate_limit_length << "s" << std::endl;
        std::cout << Translation::getByKey("lapi.noderunner.resolver.wait.start", nd->m_sInternalName, rate_limit_length) << std::endl;
    }
    std::this_thread::sleep_for(std::chrono::seconds(rate_limit_length + (int)nd->m_pPolicy->m_nResolverInterval));
    if(!nd->m_pPolicy->m_bNoOutput) {
        //std::cout << "[LevelAPI resolver " << nd->m_sInternalName << "] Rate limit end!" << std::endl;
        std::cout << Translation::getByKey("lapi.noderunner.resolver.wait.end", nd->m_sInternalName) << std::endl;
    }

    nd->m_bRateLimitApplied = false;
    return;
}

void DatabaseController::node_runner_wait_level(Node *nd, dpp::message message, int id) {
    return;
    if(nd->m_bRateLimitApplied) return;
    while(1) {
        if(nd->m_bRateLimitApplied) return;
        auto level = nd->getLevel(id);
        if(!nd->m_pPolicy->m_bNoOutput) {
            //std::cout << "[LevelAPI resolver " << nd->m_sInternalName << "] Waiting for " << id << " " << level->m_bHasLevelString << std::endl;
            std::cout << Translation::getByKey("lapi.curltest.lvlwait.start", nd->m_sInternalName, id, level->m_bHasLevelString) << std::endl;
        }
        if(level->m_bHasLevelString) {
            auto level = nd->getLevel(id);
            message.embeds.clear();
            message.add_embed(level->getAsEmbed(E_RECENT));
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

    int id = std::stoi(q->m_sText);
    float time_to_wait = nd->m_pPolicy->m_nResolverInterval - nd->m_pPolicy->m_nQueueProcessingInterval;
    if(time_to_wait < 0.f) time_to_wait = 0;
    if(!nd->m_pPolicy->m_bNoOutput) {
        std::cout << Translation::getByKey("lapi.noderunner.downloader.sleeping.fetch", nd->m_sInternalName, time_to_wait, id) << std::endl;
    }
            
    std::this_thread::sleep_for(
        std::chrono::milliseconds(
            (int)(time_to_wait * 1000.f)
        )
    );

    if(nd->m_bRateLimitApplied && nd->m_pPolicy->m_bWaitResolverRL) {
        if(!nd->m_pPolicy->m_bNoOutput) {
            std::cout << Translation::getByKey("lapi.noderunner.resolver.error.ratelimit", nd->m_sInternalName) << std::endl;
        }
        return;
    }
    if(!nd->m_pPolicy->m_bEnableResolver) {
        if(!nd->m_pPolicy->m_bNoOutput) {
            std::cout << Translation::getByKey("lapi.noderunner.resolver.error.disabled", nd->m_sInternalName) << std::endl;
        }
        return;
    }

    auto level = server->getLevelMetaByID(id, false);
    if(server->m_eStatus == GSS_PERMANENT_BAN) {
        int i = 0;
        while(i < 32) {
            std::cout << Translation::getByKey("lapi.noderunner.error.pban", nd->m_sInternalName) << std::endl;
            i++;
        }
        std::cout << Translation::getByKey("lapi.noderunner.error.pban.halt", nd->m_sInternalName) << std::endl;
        while(1);
    }
    if(level->m_nRetryAfter != 0) {
        if(!nd->m_pPolicy->m_bWaitResolverRL || level->m_nRetryAfter < 0) return;
        nd->m_nWaitTime = level->m_nRetryAfter;
        if(!nd->m_pPolicy->m_bNoOutput) {
            std::cout << Translation::getByKey("lapi.noderunner.resolver.event.ratelimit", nd->m_sInternalName, nd->m_nWaitTime) << std::endl;
        }
        nd->m_bRateLimitApplied = true;
        delete level;
        level = nullptr;
    } else {
	auto lll = nd->getLevel(level->m_nLevelID);
        nd->initLevel(level);
        level->m_bHasLevelString = true;
        level->save(lll != nullptr);
	if(lll != nullptr) {
		delete lll;
		lll = nullptr;
	}
        nd->m_jLastDownloadedLevel = level->levelJson;
        if(!nd->m_pPolicy->m_bNoOutput) {
            std::cout << Translation::getByKey("lapi.noderunner.downloader.event.levelfetched", nd->m_sInternalName, id) << std::endl;
        }
        delete level;
        level = nullptr;
    }

    delete q;
    q = nullptr;

    nd->m_bResolverIsActive = false;
}

void DatabaseController::node_runner(Node *nd) {
    int prev_q = NC_NONE;
    std::cout << Translation::getByKey("lapi.noderunner.start", nd->m_sInternalName) << std::endl;

    std::vector<int> recent_downloadedids;

    Backend::GDServer *server = nd->createServer();

    server->setDebug(false);

    std::thread rcbt(DatabaseController::node_runner_recentBot, nd);
    rcbt.detach();

    std::thread rtt(DatabaseController::node_runner_recount_task, nd);
    rtt.detach();

    Level *llevel;

    auto user_job = [=](std::string uid) {
        if(!nd->m_pPolicy->m_bNoOutput) {
            std::cout << Translation::getByKey("lapi.noderunner.downloader.user.fetch", nd->m_sInternalName) << std::endl;
        }

        int pages = 6553; //65530 levels max
        int i = 0;

        while(i < pages) {
            auto levels = server->getLevelsBySearchType(5, uid, i);
            std::cout << "Got response for " << uid << std::endl;
            if(levels.size() == 0) {
                i = pages + 1;
            } else {
                int q = 0;
                int skipped = 0;
                while(q < levels.size()) {
                    int levelid;
                    std::string levelname;
                    levelid = levels[q]->m_nLevelID;
                    levelname = levels[q]->m_sLevelName;
                    levels[q]->m_sLinkedNode = nd->m_sInternalName;

                    auto level_from_node = nd->getLevel(levelid);

                    if(level_from_node == nullptr) {
                        nd->initLevel(levels[q]);
                        levels[q]->m_bHasLevelString = false;
                        levels[q]->save();
                        nd->m_jLastDownloadedLevel = levels[q]->levelJson;
                        if (!DatabaseController::database->m_sRegisteredCID2.empty() && DatabaseController::database->m_bBotReady) {
                            DatabaseController::database->m_pLinkedBot->m_pBot->message_create(dpp::message(
                                dpp::snowflake(DatabaseController::database->m_sRegisteredCID2), levels[q]->getAsEmbed(E_REGISTERED)
                            ));
                        }
                        if(!nd->m_pPolicy->m_bNoOutput) {
                            std::cout << Translation::getByKey("lapi.noderunner.downloader.event.complete.noresolver", nd->m_sInternalName, levelid, levelname) << std::endl;
                        }
                    } else {
                        delete level_from_node;
                        skipped++;
                    }

                    delete levels[q];
                    levels[q] = nullptr;
                    q++;
                }
                std::cout << skipped << " levels were skipped (already exists)" << std::endl;
                levels.clear();
            }
            i++;
            std::this_thread::sleep_for(
                std::chrono::milliseconds((int)(nd->m_pPolicy->m_nQueueProcessingInterval * 1000.f))
            );
        }
    };

    goto start_linear;

error_ignore:
    if(!nd->m_pPolicy->m_bNoOutput) {
	    prev_q = NC_NONE;
        //std::cout << Translation::getByKey("lapi.noderunner.downloader.error.queue.empty", nd->m_sInternalName) << std::endl;
    }
    goto run_again;
loop_readonly2:
    if(!nd->m_pPolicy->m_bNoOutput) {
        std::cout << Translation::getByKey("lapi.noderunner.readonly", nd->m_sInternalName) << std::endl;
    }
    //delete server;
loop_readonly:
    if(!nd->m_pPolicy->m_bNoOutput) {
        std::cout << Translation::getByKey("lapi.noderunner.readonly", nd->m_sInternalName) << std::endl;
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
                std::cout << Translation::getByKey("lapi.noderunner.resolver.linear.wait", nd->m_sInternalName, nd->m_pPolicy->m_nResolverInterval) << std::endl;
            }

            std::this_thread::sleep_for(
                std::chrono::milliseconds(
                    (int)(nd->m_pPolicy->m_nResolverInterval * 1000.f)
                )
            );

            if(!nd->m_pPolicy->m_bNoOutput) {
                std::cout << Translation::getByKey("lapi.noderunner.resolver.linear.fetch", nd->m_sInternalName, nd->m_uQueue->m_nRuntimeState) << std::endl;
            }

            llevel = server->getLevelMetaByID(nd->m_uQueue->m_nRuntimeState, false);

            if(server->m_eStatus == GSS_PERMANENT_BAN) {
                int i = 0;
                while(i < 32) {
                    std::cout << Translation::getByKey("lapi.noderunner.error.pban", nd->m_sInternalName) << std::endl;
                    i++;
                }
                std::cout << Translation::getByKey("lapi.noderunner.error.pban.halt", nd->m_sInternalName) << std::endl;
                while(1);
            }

            if (llevel->m_nRetryAfter == 0) {
                nd->initLevel(llevel);
                llevel->m_bHasLevelString = true;
                llevel->save();
                nd->m_jLastDownloadedLevel = llevel->levelJson;
                delete llevel;
                llevel = nullptr;
                //if(!nd->m_pPolicy->m_bNoOutput) std::cout << "[LevelAPI linear resolver " << nd->m_sInternalName << "] Fetched level " << nd->m_uQueue->m_nRuntimeState << std::endl;
                if(!nd->m_pPolicy->m_bNoOutput) {
                    std::cout << Translation::getByKey("lapi.noderunner.resolver.linear.event.fetch.end", nd->m_sInternalName, nd->m_uQueue->m_nRuntimeState) << std::endl;
                }
            } else if (llevel->m_nRetryAfter > 0 && nd->m_pPolicy->m_bWaitResolverRL) {
                nd->m_nWaitTime = llevel->m_nRetryAfter;
                //if(!nd->m_pPolicy->m_bNoOutput) std::cout << "[LevelAPI linear resolver " << nd->m_sInternalName << "] RATE LIMIT for " << nd->m_nWaitTime << "s" << std::endl;
                if(!nd->m_pPolicy->m_bNoOutput) {
                    std::cout << Translation::getByKey("lapi.noderunner.resolver.linear.event.ratelimit", nd->m_sInternalName, nd->m_nWaitTime) << std::endl;
                }
                nd->m_bRateLimitApplied = true;
                delete llevel;
                llevel = nullptr;
            }

            nd->m_uQueue->m_nRuntimeState++;   
        } else {
            if(!nd->m_pPolicy->m_bNoOutput) {
                std::cout << Translation::getByKey("lapi.noderunner.resolver.linear.error.disabled", nd->m_sInternalName) << std::endl;
            }
        }
        goto run_again;
    }
start:
    if(!nd->m_pPolicy->m_bEnableLinearResolver) {
        float t = (prev_q != NC_ID && prev_q != NC_NONE) ? nd->m_pPolicy->m_nQueueProcessingInterval : 0.0f; 
        // if(!nd->m_pPolicy->m_bNoOutput) std::cout << "[LevelAPI downloader " << nd->m_sInternalName << "] Waiting " << nd->m_pPolicy->m_nQueueProcessingInterval << "s" << std::endl;
        if(!nd->m_pPolicy->m_bNoOutput) {
                //std::cout << Translation::getByKey("lapi.noderunner.downloader.wait", nd->m_sInternalName, t) << std::endl;
            }
        
            std::this_thread::sleep_for(
                std::chrono::milliseconds((int)(t * 1000.f))
            );
    }

    if(nd->m_uQueue->m_vCommandQueue.size() == 0) goto error_ignore;

    auto q = nd->m_uQueue->m_vCommandQueue.at(0);

    nd->m_uQueue->currentState = q->m_nCommand;
    prev_q = q->m_nCommand;

    switch(q->m_nCommand) {
        case NC_EXPERIMENT1: { // downloads levels from all gd users (or, atleast, tried to do so)
            if(nd->m_nExperiment1Value < 0) nd->m_nExperiment1Value = 0;
            int n = nd->m_nExperiment1Value;
            for(;;) {
                user_job(std::to_string(n));
                n++;
                nd->m_nExperiment1Value = n;
            }
            break;
        }
        case NC_USER: {
            //if(server->m_eStatus == GSS_PERMANENT_BAN) break;

            user_job(q->m_sText);

            break;
        }
        case NC_RECENT: {
            if(!nd->m_pPolicy->m_bEnableRecentTab) break;

            if(!nd->m_pPolicy->m_bNoOutput) {
                std::cout << Translation::getByKey("lapi.noderunner.downloader.recenttab.fetch", nd->m_sInternalName) << std::endl;
            }
            auto levels = server->getLevelsBySearchType(4);
            int i = 0;
            if(server->m_eStatus == GSS_PERMANENT_BAN) {
                while(i < 32) {
                    std::cout << Translation::getByKey("lapi.noderunner.error.pban", nd->m_sInternalName) << std::endl;
                    i++;
                }
                std::cout << Translation::getByKey("lapi.noderunner.error.pban.halt", nd->m_sInternalName) << std::endl;
                // rcbt.
                for(;;);
            }

            while(i < levels.size()) {
                int levelid;
                std::string levelname;
                levelid = levels[i]->m_nLevelID;
                levelname = levels[i]->m_sLevelName;
                levels[i]->m_sLinkedNode = nd->m_sInternalName;

                auto level_from_node = nd->getLevel(levelid);

                if(level_from_node == nullptr) {
                    if(!nd->userIDExists(levels[i]->m_nPlayerID) && server->getServerIdentifier() != "gdserver_boomlings") {
                        // nd->m_uQueue->m_vCommandQueue.push_back(new NodeCommandQueue(NC_USER, std::to_string(levels[i]->m_nPlayerID)));
                    }
                    nd->initLevel(levels[i]);
                    levels[i]->m_bHasLevelString = false;
                    levels[i]->save();
                    nd->m_jLastDownloadedLevel = levels[i]->levelJson;
                    recent_downloadedids.push_back(levelid);
                    if(!nd->m_bRateLimitApplied && nd->m_pPolicy->m_bEnableResolver) {
                        nd->m_uQueue->m_vCommandQueue.push_back(new NodeCommandQueue(NC_ID, std::to_string(levels[i]->m_nLevelID)));
                    }
                    if (!DatabaseController::database->m_sRegisteredCID.empty() && DatabaseController::database->m_bBotReady) {
                        DatabaseController::database->m_pLinkedBot->m_pBot->message_create(dpp::message(
                            dpp::snowflake(DatabaseController::database->m_sRegisteredCID), levels[i]->getAsEmbed(E_RECENT)
                        ));
                    }
                    if(!nd->m_pPolicy->m_bNoOutput) {
                        std::cout << Translation::getByKey("lapi.noderunner.downloader.event.complete.noresolver", nd->m_sInternalName, levelid, levelname) << std::endl;
                    }
                } else {
                    if(nd->m_pPolicy->m_bEnableResolver && !levels[i]->m_bHasLevelString) {
                        nd->m_uQueue->m_vCommandQueue.push_back(new NodeCommandQueue(NC_ID, std::to_string(levels[i]->m_nLevelID)));
                    }
                    delete level_from_node;
		        }

                delete levels[i];
                levels[i] = nullptr;

                // if(!nd->m_pPolicy->m_bNoOutput) std::cout << "[LevelAPI downloader " << nd->m_sInternalName << "] Level saved without level string: " << levelid << " \"" << levelname << "\"" << std::endl;
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
    nd->m_uQueue->m_vCommandQueue.erase(nd->m_uQueue->m_vCommandQueue.begin());
    nd->m_uQueue->save();

    nd->m_uQueue->currentState = NC_NONE;

    goto run_again;
}
