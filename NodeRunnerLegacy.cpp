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

#include "curl_backend.h"
#include "DatabaseControllerThreads.h"

#include "json/single_include/nlohmann/json.hpp"
#include <chrono>
#include <cstdio>
#include <filesystem>
#include <fstream>
#include <stdexcept>
#include <string>
#include <thread>
#include <iostream>
#include <vector>
#include <dirent.h>
#include <sys/statvfs.h>

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
    auto start = std::chrono::high_resolution_clock::now();
    std::string folder = "database/nodes/" + nd->m_sInternalName + "/levels";
    nd->m_vCachedLevels.clear();

    struct statvfs buf;
    statvfs(folder.c_str(), &buf);

    int files_allocate = ((buf.f_files - buf.f_ffree) / 2);

    nd->m_vCachedLevels.reserve(files_allocate);
    
    DIR *d = opendir(folder.c_str());
    struct dirent *dir;
    while ((dir = readdir(d)) != NULL) {
        nd->m_vCachedLevels.push_back(std::atoi(dir->d_name + 6));
    }
    closedir(d);
    
    nd->m_nCachedLevels = nd->m_vCachedLevels.size() - 2;

    if(!nd->m_pPolicy->m_bNoOutput) {
        std::cout << Translation::getByKey("lapi.noderunner.recount.complete", nd->m_sInternalName, nd->m_nCachedLevels) << std::endl;
    }

    auto stop = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(stop - start);
 
    std::cout << Translation::getByKey("lapi.noderunner.recount.time", nd->m_sInternalName, duration.count()) << std::endl;

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

void DatabaseController::node_runner_wait_level(Node *nd, dpp::message message, int id) {}

void DatabaseController::node_runner_resolve_level(Node *nd, NodeCommandQueue *q, Backend::GDServer *server) {}

void DatabaseController::node_runner(Node *nd) {
    int prev_q = NC_NONE;
    std::cout << Translation::getByKey("lapi.noderunner.start", nd->m_sInternalName) << std::endl;

    std::vector<LevelAPI::Backend::CurlProxy> proxies = {};

    int i = 0;

    while(i < nd->m_pProxy->m_vProxies.size()) {
        proxies.push_back(nd->m_pProxy->m_vProxies[i]);

        i++;
    }

    Backend::GDServer *server = nd->createServer();

    server->setDebug(false);

    std::thread rcbt(DatabaseController::node_runner_recentBot, nd);
    rcbt.detach();

    std::thread rtt(DatabaseController::node_runner_recount_task, nd);
    rtt.detach();

    Level *llevel;

    auto resolver_job = [&](Node *this_node, Backend::GDServer *this_server, std::vector<LevelAPI::Backend::CurlProxy> proxy_list) {
        if (!this_node->m_pPolicy->m_bEnableResolver) return;

        while (true) {
            if (this_node->m_uQueue->m_vResolverQueuedLevels.size() != 0) {
                printf("qsize %lu\n", this_node->m_uQueue->m_vResolverQueuedLevels.size());
                int i = 0;  
                bool dont_stop = false;
                while (i < proxy_list.size() && (i < this_node->m_uQueue->m_vResolverQueuedLevels.size() || dont_stop)) {
                    int id = this_node->m_uQueue->m_vResolverQueuedLevels[0];

                    auto selected_proxy = proxy_list[i];

                    std::string datapath = "database/nodes/" + this_node->m_sInternalName + "/" + this_node->m_sLevelDataPath + "/Level_" + std::to_string(id) + "/data.gmd2";

                    if (!std::filesystem::exists(datapath)) {
                        std::cout << "Selecting proxy " << selected_proxy.getURL() << std::endl;
                        std::cout << Translation::getByKey("lapi.noderunner.resolver.event.download.progress", this_node->m_sInternalName, id) << std::endl;

                        auto level = this_server->getLevelMetaByID(id, false, selected_proxy);

                        if (!level || level->m_nRetryAfter > 0) {
                            std::cout << "failed to download the level (proxy " << selected_proxy.getURL() << ")" << std::endl;;
                            dont_stop = true;

                            if (level != nullptr) {
                                std::cout << "reason: rate limit " << level->m_nRetryAfter << " seconds" << std::endl;
                                delete level;
                                level = nullptr;
                            }
                        } else {
                            if (level->m_nRetryAfter < 0) {
                                std::cout << "failed to download the level (proxy " << selected_proxy.getURL() << ") : level does not exist" << std::endl;;
                            } else {
                                level->m_bHasLevelString = true;
                                // std::cout << "level string: " << level->m_sLevelString << std::endl;;
                                bool exists = this_node->levelExists(id);

                                this_node->initLevel(level);
                                level->save(exists);

                                std::cout << Translation::getByKey("lapi.noderunner.resolver.event.download.complete", this_node->m_sInternalName, id) << std::endl;
                            }

                            delete level;
                            level = nullptr;
                            
                            this_node->m_uQueue->m_vResolverQueuedLevels.erase(this_node->m_uQueue->m_vResolverQueuedLevels.begin());

                            dont_stop = false;
                        }   
                    } else {
                        this_node->m_uQueue->m_vResolverQueuedLevels.erase(this_node->m_uQueue->m_vResolverQueuedLevels.begin());
                    }

                    i++;
                }

                if (this_node->m_uQueue->m_vResolverQueuedLevels.size() != 0 && !this_node->m_pPolicy->m_bUseProxyOnly) {
                    int id = this_node->m_uQueue->m_vResolverQueuedLevels[0];

                    std::string datapath = "database/nodes/" + this_node->m_sInternalName + "/" + this_node->m_sLevelDataPath + "/Level_" + std::to_string(id) + "/data.gmd2";

                    if (!std::filesystem::exists(datapath)) {
                        std::cout << Translation::getByKey("lapi.noderunner.resolver.event.download.progress", this_node->m_sInternalName, id) << std::endl;

                        auto level = this_server->getLevelMetaByID(id, false);

                        if (!level || level->m_nRetryAfter > 0) {
                            std::cout << "failed to download the level" << std::endl;

                            if (level != nullptr) {
                                std::cout << "reason: rate limit " << level->m_nRetryAfter << " seconds" << std::endl;
                                delete level;
                                level = nullptr;
                            }
                        } else {
                            if (level->m_nRetryAfter < 0) {
                                std::cout << "failed to download the level : level does not exist" << std::endl;;
                            } else {
                                level->m_bHasLevelString = true;
                                // std::cout << "level string: " << level->m_sLevelString << std::endl;;
                                bool exists = this_node->levelExists(id);

                                this_node->initLevel(level);
                                level->save(exists);

                                std::cout << Translation::getByKey("lapi.noderunner.resolver.event.download.complete", this_node->m_sInternalName, id) << std::endl;
                            }

                            delete level;
                            level = nullptr;
                            
                            this_node->m_uQueue->m_vResolverQueuedLevels.erase(this_node->m_uQueue->m_vResolverQueuedLevels.begin());
                        }
                    } else {
                        this_node->m_uQueue->m_vResolverQueuedLevels.erase(this_node->m_uQueue->m_vResolverQueuedLevels.begin());
                    }
                }
            }

            std::this_thread::sleep_for(std::chrono::seconds((int)this_node->m_pPolicy->m_nResolverInterval));
        }
    };

    std::thread resolver_thread(resolver_job, nd, server, proxies);
    resolver_thread.detach();

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

                    if(!nd->levelExists(levelid)) {
                        // nd->m_uQueue->m_vResolverQueuedLevels.push_back(levelid);
                        nd->initLevel(levels[q]);
                        levels[q]->m_bHasLevelString = false;
                        levels[q]->save();
                        if (nd->m_pPolicy->m_bEnableResolver) {
                            nd->m_uQueue->m_vResolverQueuedLevels.push_back(levels[q]->m_nLevelID);   
                        }
                        if (!DatabaseController::database->m_sRegisteredCID2.empty() && DatabaseController::database->m_bBotReady && DatabaseController::database->m_pLinkedBot->m_pBot != nullptr) {
                            DatabaseController::database->m_pLinkedBot->m_pBot->message_create(dpp::message(
                                dpp::snowflake(DatabaseController::database->m_sRegisteredCID2), levels[q]->getAsEmbed(E_REGISTERED)
                            ));
                        }
                        if(!nd->m_pPolicy->m_bNoOutput) {
                            std::cout << Translation::getByKey("lapi.noderunner.downloader.event.complete.noresolver", nd->m_sInternalName, levelid, levelname) << std::endl;
                        }
                    } else {
                        skipped++;
                    }

                    delete levels[q];
                    levels[q] = nullptr;
                    q++;
                }

                if (levels.size() < 10) {
                    i = pages + 1;
                }

                std::cout << Translation::getByKey("lapi.noderunner.downloader.skipped", nd->m_sInternalName, skipped) << std::endl;
                levels.clear();
            }
            i++;
            std::this_thread::sleep_for(
                std::chrono::milliseconds((int)(nd->m_pPolicy->m_nQueueProcessingInterval * 1000.f))
            );
        }
    };

    goto run_again;

error_ignore:
    std::this_thread::sleep_for(100ms);
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
    nd->m_bRateLimitApplied = false;
    // if(nd->m_bRateLimitApplied) {
    //     std::thread rlt(DatabaseController::node_runner_waitResolverRL, nd, nd->m_nWaitTime);
    //     rlt.detach();
    // }
start:
    if(nd->m_uQueue->m_vCommandQueue.size() == 0) goto error_ignore;

    auto q = nd->m_uQueue->m_vCommandQueue.at(0);

    nd->m_uQueue->currentState = q->m_nCommand;
    prev_q = q->m_nCommand;

    bool wait_after_command = true;

    switch(q->m_nCommand) {
        // case NC_EXPERIMENT1: { // downloads levels from all gd users (or, atleast, tries to do so)
            // break;
            // if(nd->m_nExperiment1Value < 0) nd->m_nExperiment1Value = 0;
            // int n = nd->m_nExperiment1Value;
            // for(;;) {
                // user_job(std::to_string(n));
                // n++;
                // nd->m_nExperiment1Value = n;
            // }
            // break;
        // }
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
            if(server->m_eStatus == LevelAPI::Backend::GSS_PERMANENT_BAN) {
                while(i < 32) {
                    std::cout << Translation::getByKey("lapi.noderunner.error.pban", nd->m_sInternalName) << std::endl;
                    i++;
                }

                std::cout << Translation::getByKey("lapi.noderunner.error.pban.halt", nd->m_sInternalName) << std::endl;
                for(;;);
            }

            while(i < levels.size()) {
                int levelid = levels[i]->m_nLevelID;
                std::string levelname = levels[i]->m_sLevelName;

                levels[i]->m_sLinkedNode = nd->m_sInternalName;

                if(!nd->levelExists(levelid)) {
                    bool userIDExists = nd->userIDExists(levels[i]->m_nPlayerID);
                    auto identifier = server->getServerIdentifier();
                    
                    if(!userIDExists) {
                        nd->m_uQueue->m_vCommandQueue.push_back(new NodeCommandQueue(NC_USER, std::to_string(levels[i]->m_nPlayerID)));
                    }

                    nd->initLevel(levels[i]);
                    levels[i]->m_bHasLevelString = false;
                    levels[i]->save();
                    // nd->m_jLastDownloadedLevel = levels[i]->_jsonObject;
                    // recent_downloadedids.push_back(levelid);
                    // if(!nd->m_bRateLimitApplied && nd->m_pPolicy->m_bEnableResolver) {
                    //     nd->m_uQueue->m_vCommandQueue.push_back(new NodeCommandQueue(NC_ID, std::to_string(levels[i]->m_nLevelID)));
                    // }
                    if (!DatabaseController::database->m_sRegisteredCID.empty() && DatabaseController::database->m_bBotReady && DatabaseController::database->m_pLinkedBot->m_pBot != nullptr) {
                        DatabaseController::database->m_pLinkedBot->m_pBot->message_create(dpp::message(
                            dpp::snowflake(DatabaseController::database->m_sRegisteredCID), levels[i]->getAsEmbed(E_RECENT)
                        ));
                    }
                    if(!nd->m_pPolicy->m_bNoOutput) {
                        std::cout << Translation::getByKey("lapi.noderunner.downloader.event.complete.noresolver", nd->m_sInternalName, levelid, levelname) << std::endl;
                    }
                    // printf("level %d does not exist\n", levelid);
                    if(nd->m_pPolicy->m_bEnableResolver) nd->m_uQueue->m_vResolverQueuedLevels.push_back(levelid);
                } else {
                    if(nd->m_pPolicy->m_bEnableResolver && !levels[i]->m_bHasLevelString) {
                        // nd->m_uQueue->m_vCommandQueue.push_back(new NodeCommandQueue(NC_ID, std::to_string(levels[i]->m_nLevelID)));
                    }
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
            nd->m_uQueue->m_vResolverQueuedLevels.push_back(std::stoi(q->m_sText));
        }
        default:
        case NC_NONE:
        case NC_IDLE: {
            wait_after_command = false;

            break;
        }
    }
    
    delete q;
    q = nullptr;
    
    nd->m_uQueue->m_vCommandQueue.erase(nd->m_uQueue->m_vCommandQueue.begin());
    // nd->m_uQueue->save();

    nd->m_uQueue->currentState = NC_NONE;

    goto run_again;
}
