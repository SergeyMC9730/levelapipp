#pragma once

#include "lapi_database.h"

namespace LevelAPI {
    namespace NodeController {
        class NodeRunner {
        public:
            DatabaseController::Node *m_pLinkedNode;
            int m_nPreviousRequest;
            
            void processLinear();

            void errorReadOnly();
            void errorEmptyQueue();
        
            static void thread_downloader(NodeRunner *self);
            static void thread_resolver(NodeRunner *self);
            static void thread_cacheLevels(NodeRunner *self);
            static void thread_pushRecentTab(NodeRunner *self);
            
            static void task_waitRatelimit(NodeRunner *self, int rate_limit_length);

            void init(DatabaseController::Node *node);
        };
    }
}