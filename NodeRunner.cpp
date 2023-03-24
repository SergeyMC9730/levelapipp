#include "DatabaseControllerThreads.h"

#include <chrono>
#include <thread>
#include <iostream>

#include "GDServer_BoomlingsLike21.h"

using namespace LevelAPI;
using namespace std::chrono_literals;

void DatabaseController::node_runner(Node *nd) {
    auto server = new Backend::GDServer_BoomlingsLike21(nd->m_uDatabase->m_sEndpoint);

start:
    std::cout << "[LevelAPI downloader " << *nd->m_sInternalName << "] Waiting 1s" << std::endl;

    std::this_thread::sleep_for(1s);

    auto levels = server->getLevelsBySearchType(4);

    int waittime = 0;

    int i = 0;
    while(i < levels.size()) {
        std::cout << "[LevelAPI resolver " << *nd->m_sInternalName << "] Waiting " << waittime + 3 << "s" << std::endl; 
        std::this_thread::sleep_for(std::chrono::seconds(waittime + 3));
        
        // std::cout << "Level " << levels[i]->m_nLevelID << std::endl;

        auto level = server->resolveLevelData(levels[i]);
        waittime = level->m_nRetryAfter;
        if(waittime == 0) {
            nd->initLevel(level);
            level->save();
            std::cout << "[LevelAPI resolver " << *nd->m_sInternalName << "] Resolved level " << level->m_nLevelID << std::endl; 
        } else {
            std::cout << "[LevelAPI resolver " << *nd->m_sInternalName << "] RATE LIMIT for " << waittime << "s" << std::endl; 
        }

        // delete level;

        i++;
    }

    goto start;
}