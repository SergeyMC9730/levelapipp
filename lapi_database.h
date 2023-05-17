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
            NC_USER, NC_RECENT, NC_EXPERIMENT1,
            NC_NONE, NC_IDLE
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
        class NodeProxyList {
        public:
            nlohmann::json plJson;
        public:
            std::vector<std::string> m_vProxies;

            NodeProxyList();

            void save();
            void recover();
            
            void setupJSON();
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

            std::string m_sPlayerLogin;
            std::string m_sPlayerPassword;

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
        protected:
            Backend::GDServer *m_pCachedGDInstance = nullptr;
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
            NodeProxyList *m_pProxy;

            bool m_bCacheInProcess;
            std::vector<int> m_vCachedLevels;
            int m_nCachedLevels;

            nlohmann::json m_jLastDownloadedLevel;

            bool m_bResolverIsActive;

            int m_nWaitTime;
            int m_nExperiment1Value;

            bool m_bRateLimitApplied;

            void initLevel(Level *level);
            
            void save();
            void recover();

            void setupJSON();

            void createLevelFolder();

            Level *getLevel(int id);
            bool userIDExists(int uid);

            LevelAPI::Backend::GDServer *createServer();

            std::vector<Level *> getLevels(SearchFilter *filter);

            Node *getSelf();
            void importLevelMetaFromLAPIold(std::string path);

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
            std::string m_sRegisteredCID2;

            bool exists();

            Database(std::vector<Node *> nodes);
            Database(Node *node);
            Database(std::string path);
            Database();

            void recalculate();

            void save();

            int m_nNodeSize;
            std::vector<Node *> m_vNodes;

            int getTotalLevels();

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
