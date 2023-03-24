#include "DatabaseControllerThreads.h"

#include <chrono>
#include <thread>
#include <iostream>

#include "Tools.h"
#include "GDServer_BoomlingsLike21.h"

using namespace LevelAPI;
using namespace std::chrono_literals;

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

start:
    std::cout << "[LevelAPI downloader " << *nd->m_sInternalName << "] Waiting 2s" << std::endl;

    std::this_thread::sleep_for(1500ms);

    std::cout << "[LevelAPI downloader " << *nd->m_sInternalName << "] Fetching levels..." << std::endl;

    auto levels = server->getLevelsBySearchType(4);

    int waittime = 0;

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

            std::cout << "[LevelAPI downloader " << *nd->m_sInternalName << "] Level saved without level string (RL state): " << levelid << " \"" << levelname << "\"" << std::endl; 

            i++;
        }
        goto start;
    }

    while(i < levels.size()) {

        std::cout << "[LevelAPI resolver " << *nd->m_sInternalName << "] Waiting " << waittime + 3 << "s" << std::endl; 
        std::this_thread::sleep_for(std::chrono::seconds(waittime + 3));
        
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
            std::cout << "[LevelAPI resolver " << *nd->m_sInternalName << "] Resolved level " << levelid << " \"" << levelname << "\"" << std::endl; 
        } else {
            std::cout << "[LevelAPI resolver " << *nd->m_sInternalName << "] RATE LIMIT for " << waittime << "s" << std::endl;
            // level->m_bHasLevelString = false;
            // nd->initLevel(level);
            // level->save();
            nd->m_bRateLimitApplied = true;
            break;
        }

        i++;
    }

    if(nd->m_bRateLimitApplied) {
        std::thread rlt(DatabaseController::node_runner_waitResolverRL, nd, waittime);
        rlt.detach();
    }

    goto start;
}