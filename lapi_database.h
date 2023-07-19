#pragma once

#include "DatabaseCell.h"
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

#include "fast-vector/fast_vector.h"

#include "SQLiteManager.h"

#include "DatabaseCell.h"

// #pragma pack(push, 1)

namespace LevelAPI {
    namespace DatabaseController {
        enum NodeCommands {
            NC_ID = 0, 
            NC_USER, NC_RECENT, NC_EXPERIMENT1,
            NC_NONE, NC_IDLE
        };

        class NodeCommandQueue : public DatabaseCell {
        public:
            NodeCommandQueue(int command, std::string text);
            NodeCommandQueue();

            uint8_t m_nCommand;
            std::string m_sText;

            void save() override;
            void recover() override;

            void setupJSON() override;

            ~NodeCommandQueue();
        };
        class NodeQueue : public DatabaseCell {
        public:
            NodeQueue(NodeCommandQueue *q, bool executeQueue, int runtimeState);
            NodeQueue(std::vector<NodeCommandQueue *> vec, bool executeQueue, int runtimeState);
            NodeQueue(bool executeQueue, int runtimeState);
            NodeQueue();

            std::vector<NodeCommandQueue *> m_vCommandQueue = {};
            std::vector<int> m_vResolverQueuedLevels = {};
    
            bool m_bExecuteQueue;
            int m_nRuntimeState;

            uint8_t currentState;

            void save() override;
            void recover() override;

            void setupJSON() override;

            ~NodeQueue();
        };
        class NodeProxyList : public DatabaseCell {
        public:
            std::vector<std::string> m_vProxies;

            NodeProxyList();

            void save() override;
            void recover() override;
            
            void setupJSON() override;
        };
        class NodeDatabase : public DatabaseCell {
        public:
            NodeDatabase();
            NodeDatabase(std::string endpoint, uint8_t featureSet, bool readOnly);

            std::string m_sEndpoint;
            uint8_t m_nFeatureSet;
            std::string m_sModifications;
            bool m_bReadOnly;

            std::string m_sPlayerLogin;
            std::string m_sPlayerPassword;

            int m_nLevels = 0;

            void save() override;
            void recover() override;

            void setupJSON() override;

            std::vector<std::string> getModifications();

            ~NodeDatabase();
        };
        class NodePolicy : public DatabaseCell {
        public:
            NodePolicy();

            bool m_bEnableRecentTab;
            bool m_bWaitResolverRL;
            bool m_bEnableLinearResolver;
            bool m_bEnableResolver; // only level meta would be downloaded if resolver is disabled
            bool m_bUseProxyOnly;
            float m_nResolverInterval;
            float m_nQueueProcessingInterval;
            bool m_bNoOutput;

            void save() override;
            void recover() override;

            void setupJSON() override;
        };
        class Node : public DatabaseCell {
        protected:
            Backend::GDServer *m_pCachedGDInstance = nullptr;
        public:
            Node(NodeDatabase *database, std::string internalName, std::string levelDataPath, NodeQueue *queue);
            Node(NodeDatabase *database, std::string internalName, std::string levelDataPath);
            Node(std::string internalName = "node");

            NodeDatabase *m_uDatabase;
            std::string m_sInternalName;
            std::string m_sLevelDataPath;
            NodeQueue *m_uQueue;
            NodePolicy *m_pPolicy;
            NodeProxyList *m_pProxy;

            bool m_bCacheInProcess;
            fast_vector<int> m_vCachedLevels;
            int m_nCachedLevels;

            nlohmann::json m_jLastDownloadedLevel;

            bool m_bResolverIsActive;

            int m_nWaitTime;
            int m_nExperiment1Value;

            bool m_bRateLimitApplied;

            static nlohmann::json jsonFromSQLLevel(SQLiteServerRow row); 

            void initLevel(Level *level);
            
            void save() override;
            void recover() override;

            void setupJSON() override;
            void setupSQLite() override;

            void createLevelFolder();

            Level *getLevel(int id);
            bool levelExists(int id);

            bool userIDExists(int uid);

            LevelAPI::Backend::GDServer *createServer();

            std::vector<Level *> getLevels(SearchFilter *filter);
            std::vector<int> getIDs(SearchFilter *filter);

            void importLevelMetaFromLAPIold(std::string path);

            ~Node();
        };
        class Database : public DatabaseCell {
        private:
            bool m_bLoadedFromBackup = false;
        public:
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

            void save() override;

            int m_nNodeSize;
            std::vector<Node *> m_vNodes;

            int getTotalLevels();

            void setupJSON() override;

            Node *getNode(std::string internalName);

            void runThreads();

            ~Database();
        };

        extern Database *database;

        void setup();
    }
}

// #pragma pack(pop)
