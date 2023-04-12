#pragma once

#include "cluster.h"
#include <dpp/dpp.h>

#include <thread>

namespace LevelAPI {
    namespace DiscordController {
        class DiscordInstance {
        public:
            dpp::cluster *m_pBot;
            void *m_pDB;

            DiscordInstance(void *db);
            static void dthread(DiscordInstance *instance); 

            std::thread *start();     
        };
    }
}