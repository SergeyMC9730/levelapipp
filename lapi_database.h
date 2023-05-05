#pragma once

#include "cluster.h"
#include <array>
#include <cstdint>
#include <string>
#include <thread>
#include <vector>

#include "gmd2pp/GJGameLevel.h"

#include "json/single_include/nlohmann/json.hpp"

#include <dpp/dpp.h>

#include "DiscordInstance.h"

#include "SearchFilter.h"

#include "GDServer.h"

#pragma pack(push, 1)

namespace LevelAPI {
    namespace DatabaseController {
        enum NodeCommands {
            NC_ID = 0,
            NC_USER = 1,
            NC_RECENT = 2,
            NC_NONE = 3,
            NC_IDLE = 4
        };

        class NodeCommandQueue {
        public:
            nlohmann::json commandJson;
        public:
            NodeCommandQueue(int command, std::string text);
            NodeCommandQueue();

            uint8_t m_nCommand;
            std::string m_sText;

            void save();
            void recover();

            void setupJSON();

            ~NodeCommandQueue();
        };
        class NodeQueue {
        public:
            nlohmann::json queueJson;
        public:
            NodeQueue(NodeCommandQueue *q, bool executeQueue, int runtimeState);
            NodeQueue(std::vector<NodeCommandQueue *> vec, bool executeQueue, int runtimeState);
            NodeQueue(bool executeQueue, int runtimeState);
            NodeQueue();

            std::vector<NodeCommandQueue *> m_vCommandQueue;
            bool m_bExecuteQueue;
            int m_nRuntimeState;

            uint8_t currentState;

            void save();
            void recover();

            void setupJSON();

            ~NodeQueue();
        };
        class NodeDatabase {
        public:
            nlohmann::json ndJson;
        public:
            NodeDatabase();
            NodeDatabase(std::string endpoint, uint8_t featureSet, bool readOnly);

            std::string m_sEndpoint;
            uint8_t m_nFeatureSet;
            std::string m_sModifications;
            bool m_bReadOnly;

            int m_nLevels;

            void save();
            void recover();

            void setupJSON();

            std::vector<std::string> getModifications();

            ~NodeDatabase();
        };
        class NodePolicy {
        public:
            nlohmann::json policyJson;

            NodePolicy();

            bool m_bEnableRecentTab;
            bool m_bWaitResolverRL;
            bool m_bEnableLinearResolver;
            bool m_bEnableResolver; // only level meta would be downloaded if resolver is disabled
            float m_nResolverInterval;
            float m_nQueueProcessingInterval;
            bool m_bNoOutput;

            void save();
            void recover();

            void setupJSON();
        };
        class Node {
        public:
            nlohmann::json nodeJson;

            Node(NodeDatabase *database, std::string internalName, std::string levelDataPath, NodeQueue *queue);
            Node(NodeDatabase *database, std::string internalName, std::string levelDataPath);
            Node();

            NodeDatabase *m_uDatabase;
            std::string m_sInternalName;
            std::string m_sLevelDataPath;
            NodeQueue *m_uQueue;
            NodePolicy *m_pPolicy;

            bool m_bCacheInProcess;
            std::vector<int> m_vCachedLevels;

            nlohmann::json m_jLastDownloadedLevel;

            bool m_bResolverIsActive;

            int m_nWaitTime;

            bool m_bRateLimitApplied;

            void initLevel(Level *level);
            
            void save();
            void recover();

            void setupJSON();

            void createLevelFolder();

            Level *getLevel(int id);

            LevelAPI::Backend::GDServer *createServer();

            std::vector<Level *> getLevels(SearchFilter *filter);

            Node *getSelf();

            ~Node();
        };
        class Database {
        public:
            nlohmann::json databaseJson;
            std::string databasePath;
            LevelAPI::DiscordController::DiscordInstance *m_pLinkedBot;
            bool m_bEnableBot;
            std::vector<std::thread *> m_vThreads;
            std::string m_sBotToken;

            bool m_bBotReady;

            std::string m_sRegisteredCID;

            bool exists();

            Database(std::vector<Node *> nodes);
            Database(Node *node);
            Database(std::string path);
            Database();

            void recalculate();

            void save();

            int m_nNodeSize;
            std::vector<Node *> m_vNodes;

            void setupJSON();

            Node *getNode(std::string internalName);

            void runThreads();

            ~Database();
        };
        extern Database *database;
        void setup();
    }
}

#pragma pack(pop)
