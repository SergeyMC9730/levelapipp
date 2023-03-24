#pragma once

#include "gmd2pp/level-converter/GJGameLevel.h"
#include <array>
#include <cstdint>
#include <string>
#include <thread>
#include <vector>

#include "gmd2pp/level-converter/GJGameLevel.h"

#include "json/single_include/nlohmann/json.hpp"

#pragma pack(push, 1)

namespace LevelAPI {
    namespace DatabaseController {
        class LevelRelease {
        public:
            ~LevelRelease();

            int m_nGameVersion;
            std::string *m_fActualVersion;

            static std::string determineFromID(int id);
        };
        class Level : public GJGameLevel {
        public:
            nlohmann::json levelJson;
            LevelRelease *m_uRelease;
            std::string *m_sLevelPath;
            int m_nAccountID;
            std::string *m_sUsername;

            int m_nRetryAfter;
            bool m_bHasLevelString;

            void setupJSON();
            Level();
            void restore();
            void save();

            ~Level();
        };
        class NodeCommandQueue {
        public:
            nlohmann::json commandJson;
        public:
            NodeCommandQueue(int command, std::string *text);
            NodeCommandQueue();

            uint8_t m_nCommand;
            std::string *m_sText;

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
            NodeQueue(std::vector<NodeCommandQueue *> *vec, bool executeQueue, int runtimeState);
            NodeQueue(bool executeQueue, int runtimeState);
            NodeQueue();

            std::vector<NodeCommandQueue *> *m_vCommandQueue;
            bool m_bExecuteQueue;
            int m_nRuntimeState;

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
            NodeDatabase(std::string *endpoint, uint8_t featureSet, bool readOnly);

            std::string *m_sEndpoint;
            uint8_t m_nFeatureSet;
            bool m_bReadOnly;

            int m_nLevels;

            void save();
            void recover();

            void setupJSON();

            ~NodeDatabase();
        };
        class Node {
        public:
            nlohmann::json nodeJson;
        public:
            Node(NodeDatabase *database, std::string *internalName, std::string *levelDataPath, NodeQueue *queue);
            Node(NodeDatabase *database, std::string *internalName, std::string *levelDataPath);
            Node();

            NodeDatabase *m_uDatabase;
            std::string *m_sInternalName;
            std::string *m_sLevelDataPath;
            NodeQueue *m_uQueue;

            bool m_bRateLimitApplied;

            void initLevel(Level *level);
            
            void save();
            void recover();

            void setupJSON();

            void createLevelFolder();

            Level *getLevel(int id);

            Node *getSelf();

            ~Node();
        };
        class Database {
        private:
            std::vector<std::thread *> m_vThreads;
        public:
            nlohmann::json databaseJson;
            std::string *databasePath;

            bool exists();

            Database(std::vector<Node *> *nodes);
            Database(Node *node);
            Database(std::string *path);
            Database();

            void recalculate();

            void save();

            int m_nNodeSize;
            std::vector<Node *> *m_vNodes;

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