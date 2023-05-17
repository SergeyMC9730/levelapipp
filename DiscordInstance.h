#pragma once

#include "cluster.h"
#include <dpp/dpp.h>

#include <thread>

#include "DiscordController.h"

namespace LevelAPI {
    namespace DiscordController {
        class DiscordInstance {
        public:
            dpp::cluster *m_pBot;
            void *m_pDB;

            DiscordInstance(void *db);
            static void dthread(DiscordInstance *instance); 

            void setStatus(std::string status);

            std::thread *start();     
        };
    }
}