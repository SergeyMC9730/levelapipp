#include "NodeRunner.h"

#include "DatabaseControllerThreads.h"

#include <chrono>
#include <string>
#include <thread>
#include <iostream>
#include <vector>

#include "GDServer.h"
#include "GDServer_BoomlingsLike21.h"
#include "GDServer_BasementLike21.h"
#include "GDServer_BoomlingsLike19.h"
#include "lapi_database.h"
#include "message.h"
#include "restresults.h"

#include "Translation.h"

using namespace LevelAPI::NodeController;
using namespace LevelAPI::DatabaseController;
using namespace LevelAPI::Frontend;
using namespace std::chrono_literals;

void NodeRunner::errorEmptyQueue() {
    if(!m_pLinkedNode->m_pPolicy->m_bNoOutput) {
        m_nPreviousRequest = NC_NONE;
        std::cout << Translation::getByKey("lapi.noderunner.downloader.error.queue.empty", m_pLinkedNode->m_sInternalName) << std::endl;
    }
    while(true) {
        std::this_thread::sleep_for(std::chrono::seconds(2s));
    }
}
void NodeRunner::errorReadOnly() {
    if(!m_pLinkedNode->m_pPolicy->m_bNoOutput) {
        std::cout << Translation::getByKey("lapi.noderunner.readonly", m_pLinkedNode->m_sInternalName) << std::endl;
    }
}

void NodeRunner::thread_cacheLevels(NodeRunner *self) {
    while(true) {
        std::string folder = "database/nodes/" + self->m_pLinkedNode->m_sInternalName + "/levels";
        self->m_pLinkedNode->m_vCachedLevels.clear();
        for (const auto & entry : std::filesystem::directory_iterator(folder)) {
            std::string path = entry.path();
            std::string filename = path.substr(path.find_last_of("/\\") + 1);
            std::string levelid = filename.substr(filename.find_last_of("_") + 1);
            self->m_pLinkedNode->m_vCachedLevels.push_back(std::stoi(levelid));
        }
        if(!self->m_pLinkedNode->m_pPolicy->m_bNoOutput) {
            std::cout << Translation::getByKey("lapi.noderunner.recount.complete", self->m_pLinkedNode->m_sInternalName, self->m_pLinkedNode->m_vCachedLevels.size()) << std::endl;
        }
        std::this_thread::sleep_for(5s);
    }
}

void NodeRunner::task_waitRatelimit(NodeRunner *self, int rate_limit_length) {
    if(self->m_pLinkedNode->m_bRateLimitApplied) return;
    
    self->m_pLinkedNode->m_bRateLimitApplied = true;
    
    if(!self->m_pLinkedNode->m_pPolicy->m_bNoOutput) {
        std::cout << "[LevelAPI resolver " << self->m_pLinkedNode->m_sInternalName << "] Waiting " << rate_limit_length << "s" << std::endl;
        std::cout << Translation::getByKey("lapi.noderunner.resolver.wait.start", self->m_pLinkedNode->m_sInternalName, rate_limit_length) << std::endl;
    }
    std::this_thread::sleep_for(std::chrono::seconds(rate_limit_length + (int)self->m_pLinkedNode->m_pPolicy->m_nResolverInterval));
    if(!self->m_pLinkedNode->m_pPolicy->m_bNoOutput) {
        std::cout << "[LevelAPI resolver " << self->m_pLinkedNode->m_sInternalName << "] Rate limit end!" << std::endl;
        std::cout << Translation::getByKey("lapi.noderunner.resolver.wait.end", self->m_pLinkedNode->m_sInternalName) << std::endl;
    }

    self->m_pLinkedNode->m_bRateLimitApplied = false;
    return;
}

void NodeRunner::thread_pushRecentTab(NodeRunner *self) {
    if(!self->m_pLinkedNode->m_pPolicy->m_bEnableRecentTab) return;
    while(true) {
        std::this_thread::sleep_for(std::chrono::milliseconds((int)(self->m_pLinkedNode->m_pPolicy->m_nQueueProcessingInterval * 1000)));
        if(true) { // self->m_pLinkedNode->m_uQueue->m_vCommandQueue->size() == 0
            self->m_pLinkedNode->m_uQueue->m_vCommandQueue.push_back(new NodeCommandQueue(NC_RECENT, "-"));
            self->m_pLinkedNode->m_uQueue->save();
        }
    }
}