#include "DatabaseControllerThreads.h"

#include <chrono>
#include <thread>
#include <iostream>

#include "Tools.h"
#include "GDServer_BoomlingsLike21.h"
#include "lapi_database.h"

using namespace LevelAPI;
using namespace std::chrono_literals;

void DatabaseController::node_runner_recentBot(Node *nd) {
start:
    std::this_thread::sleep_for(std::chrono::seconds(2s));
    if(nd->m_uQueue->currentState == NC_NONE || nd->m_uQueue->currentState == NC_IDLE) {
        nd->m_uQueue->m_vCommandQueue->push_back(new NodeCommandQueue(NC_RECENT, new std::string("-")));
        nd->m_uQueue->save();
    }
    goto start;
}

void DatabaseController::node_runner_waitResolverRL(Node *nd, int rate_limit_length) {
    nd->m_bRateLimitApplied = true;
    
    std::cout << "[LevelAPI resolver " << *nd->m_sInternalName << "] Waiting " << rate_limit_length << "s" << std::endl; 
    std::this_thread::sleep_for(std::chrono::seconds(rate_limit_length + 3));
    std::cout << "[LevelAPI resolver " << *nd->m_sInternalName << "] Rate limit end!" << std::endl; 

    nd->m_bRateLimitApplied = false;
    return;
}

void DatabaseController::node_runner(Node *nd) {
    auto server = new Backend::GDServer_BoomlingsLike21(nd->m_uDatabase->m_sEndpoint);
    server->setDebug(true);

    std::thread rcbt(DatabaseController::node_runner_recentBot, nd);
    rcbt.detach();

    goto start;

error_ignore:
    std::cout << "[LevelAPI downloader " << *nd->m_sInternalName << "] No stuff in the queue. Skipping" << std::endl;
start:
    std::cout << "[LevelAPI downloader " << *nd->m_sInternalName << "] Waiting 2s" << std::endl;

    std::this_thread::sleep_for(1500ms);

    if(nd->m_uQueue->m_vCommandQueue->size() == 0) goto error_ignore;

    auto q = nd->m_uQueue->m_vCommandQueue->at(0);
    int waittime = 0;

    nd->m_uQueue->currentState = q->m_nCommand;

    switch(q->m_nCommand) {
        case NodeCommands::NC_RECENT: {
            std::cout << "[LevelAPI downloader " << *nd->m_sInternalName << "] Fetching recent levels..." << std::endl;
            auto levels = server->getLevelsBySearchType(4);

            int i = 0;

            if(nd->m_bRateLimitApplied) {
                while(i < levels.size()) {
                    int levelid;
                    std::string levelname;
                    levelid = levels[i]->m_nLevelID;
                    levelname = std::string(*(levels[i]->m_sLevelName));
                    
                    nd->initLevel(levels[i]);
                    levels[i]->m_bHasLevelString = false;
                    levels[i]->save();
                    nd->m_jLastDownloadedLevel = levels[i]->levelJson;

                    std::cout << "[LevelAPI downloader " << *nd->m_sInternalName << "] Level saved without level string (RL state): " << levelid << " \"" << levelname << "\"" << std::endl; 

                    i++;
                }
            } else {
                while(i < levels.size()) {
                    std::cout << "[LevelAPI resolver " << *nd->m_sInternalName << "] Waiting " << waittime + 4 << "s" << std::endl; 
                    std::this_thread::sleep_for(std::chrono::seconds(waittime + 4));
                    
                    // std::cout << "Level " << levels[i]->m_nLevelID << std::endl;

                    auto level = server->resolveLevelData(levels[i]);
                    waittime = level->m_nRetryAfter;
                    int levelid;
                    std::string levelname;
                    if(waittime == 0) {
                        levelid = level->m_nLevelID;
                        levelname = std::string(*level->m_sLevelName);
                        nd->initLevel(level);
                        level->m_bHasLevelString = true;
                        level->save();
                        nd->m_jLastDownloadedLevel = level->levelJson;
                        std::cout << "[LevelAPI resolver " << *nd->m_sInternalName << "] Resolved level " << levelid << " \"" << levelname << "\"" << std::endl; 
                    } else {
                        std::cout << "[LevelAPI resolver " << *nd->m_sInternalName << "] RATE LIMIT for " << waittime << "s" << std::endl;
                        nd->m_bRateLimitApplied = true;
                        break;
                    }

                    i++;
                }
            }
            break;
        }
        default:
        case NC_NONE:
        case NC_IDLE: break;
    }
    delete q;
    nd->m_uQueue->m_vCommandQueue->erase(nd->m_uQueue->m_vCommandQueue->begin());
    
    nd->m_uQueue->currentState = NC_NONE;

    if(nd->m_bRateLimitApplied) {
        std::thread rlt(DatabaseController::node_runner_waitResolverRL, nd, waittime);
        rlt.detach();
    }

    goto start;
}
