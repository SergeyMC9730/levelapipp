/**
 *  LevelAPI - Geometry Dash level cacher with search functionality and more.
    Copyright (C) 2023  Sergei Baigerov

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <https://www.gnu.org/licenses/>.
*/

#pragma once

#include "DatabaseCell.h"
#include <array>
#include <cstdint>
#include <string>
#include <thread>
#include <vector>

#include "json/single_include/nlohmann/json.hpp"

#ifdef _DPP_ENABLED_
#include <dpp/dpp.h>
#include "DiscordInstance.h"
#endif

#include "SearchFilter.h"

#include "GDServer.h"

#include "SQLiteManager.h"

#include "DatabaseCell.h"

// #pragma pack(push, 1)

namespace LevelAPI {
    namespace DatabaseController {
        enum NodeCommands {
            NC_ID = 0, 
            NC_USER, NC_RECENT, NC_EXPERIMENT1,
            NC_NONE, NC_IDLE,
            NC_EXPERIMENT2, NC_22REGION_META
        };

        class NodeCommandQueue : public DatabaseCell {
        public:
            NodeCommandQueue(int command, std::string text);
            NodeCommandQueue();

            uint8_t m_nCommand = 0;
            std::string m_sText = "";

            void save() override;
            void recover() override;

            void setupJSON() override;

            ~NodeCommandQueue();
        };
        class NodeQueue : public DatabaseCell {
        public:
            NodeQueue(const NodeCommandQueue &q, bool executeQueue, int runtimeState);
            NodeQueue(const std::vector<NodeCommandQueue> &vec, bool executeQueue, int runtimeState);
            NodeQueue(bool executeQueue, int runtimeState);
            NodeQueue();

            std::vector<NodeCommandQueue> m_vCommandQueue = {};
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

            static std::vector<std::string> getModificationsList();
            static std::vector<int> getFeatureSetsList();

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

        struct NodeGraphGen {
            std::vector<int> _values;
            std::string _filename;
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
            NodeProxyList *m_pProxyDownloader;

            bool m_bCacheInProcess;
            std::vector<int> m_vCachedLevels;
            int m_nCachedLevels;

            nlohmann::json m_jLastDownloadedLevel;

            bool m_bResolverIsActive;

            int m_nWaitTime;
            int m_nExperiment1Value;

            bool m_bRateLimitApplied;

            std::vector<struct NodeGraphGen> m_vLastGraphs;

            static nlohmann::json jsonFromSQLLevel(SQLiteServerRow &row); 

            void initLevel(Level *level);
            
            void save() override;
            void recover() override;

            void setupJSON() override;
            void setupSQLite() override;

            void createLevelFolder();

            Level *getLevel(int id);
            bool levelExists(int id);

            bool userIDExists(int uid);

            void createGraph(std::vector<int> levels_per_hour, std::string filename);

            LevelAPI::Backend::GDServer *createServer();

            std::vector<Level *> getLevels(LevelAPI::Backend::SearchFilter filter);
            std::vector<int> getIDs(LevelAPI::Backend::SearchFilter filter);

            void importLevelMetaFromLAPIold(std::string path);

            std::string getLevelPathRepresentation(int id);

            ~Node();
        };
        class Database : public DatabaseCell {
        private:
            bool m_bLoadedFromBackup = false;
        public:
            std::string databasePath;
#ifdef _DPP_ENABLED_
            LevelAPI::Frontend::DiscordInstance *m_pLinkedBot;
            bool m_bEnableBot;
            std::string m_sBotToken;
            bool m_bBotReady;
            std::string m_sRegisteredCID;
            std::string m_sRegisteredCID2;
#endif
            std::vector<std::thread *> m_vThreads;

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
        void make_directories();
    }
}

// #pragma pack(pop)
