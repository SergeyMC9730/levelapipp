#pragma once

#include <array>
#include <cstdint>
#include <string>
#include <vector>

#include "json/single_include/nlohmann/json.hpp"

#pragma pack(push, 1)

namespace LevelAPI {
    namespace DatabaseController {
        namespace HttpController {
            void parse();
            int getPort();
        }
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
        };
        class NodeQueue {
        public:
            nlohmann::json queueJson;
        public:
            NodeQueue(NodeCommandQueue q, bool executeQueue, int runtimeState);
            NodeQueue(std::vector<NodeCommandQueue> vec, bool executeQueue, int runtimeState);
            NodeQueue(bool executeQueue, int runtimeState);
            NodeQueue();

            std::vector<NodeCommandQueue> m_vCommandQueue;
            bool m_bExecuteQueue;
            int m_nRuntimeState;

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
            bool m_bReadOnly;

            int m_nLevels;

            void save();
            void recover();

            void setupJSON();
        };
        class Node {
        public:
            nlohmann::json nodeJson;
        public:
            Node(NodeDatabase database, std::string internalName, std::string levelDataPath, NodeQueue queue);
            Node(NodeDatabase database, std::string internalName, std::string levelDataPath);
            Node();

            NodeDatabase m_uDatabase;
            std::string m_sInternalName;
            std::string m_sLevelDataPath;
            NodeQueue m_uQueue;
            
            void save();
            void recover();

            void setupJSON();
        };
        class Database {
        public:
            nlohmann::json databaseJson;
            std::string databasePath;
        public:
            bool exists();

            Database(std::vector<Node> nodes);
            Database(Node node);
            Database(std::string path);
            Database();

            void recalculate();

            void save();
            
            int m_nNodeSize;
            std::vector<Node> m_vNodes;

            void setupJSON();
        };
        extern Database *database;
        void setup();
    }
}

#pragma pack(pop)