#pragma once

#include <array>
#include <cstdint>
#include <string>
#include <vector>

#pragma pack(push, 1)

namespace LevelAPI {
    namespace DatabaseController {
        namespace HttpController {
            void parse();
            int getPort();
        }
        class NodeCommandQueue {
        private:

        public:
            NodeCommandQueue(int command, std::string text);
            NodeCommandQueue();

            uint8_t m_nCommand;
            std::string m_sText;
        };
        class NodeQueue {
        private:

        public:
            NodeQueue(NodeCommandQueue q, bool executeQueue, int runtimeState);
            NodeQueue(std::vector<NodeCommandQueue> vec, bool executeQueue, int runtimeState);
            NodeQueue(bool executeQueue, int runtimeState);
            NodeQueue();

            std::vector<NodeCommandQueue> m_vCommandQueue;
            bool m_bExecuteQueue;
            int m_nRuntimeState;
        };
        class NodeDatabase {
        private:

        public:
            NodeDatabase();
            NodeDatabase(std::string endpoint, uint8_t featureSet, bool readOnly);

            std::string m_sEndpoint;
            uint8_t m_nFeatureSet;
            bool m_bReadOnly;
        };
        class Node {
        private:

        public:
            Node(NodeDatabase database, std::string internalName, std::string levelDataPath, NodeQueue queue);
            Node(NodeDatabase database, std::string internalName, std::string levelDataPath);
            Node();

            NodeDatabase m_uDatabase;
            std::string m_sInternalName;
            std::string m_sLevelDataPath;
            NodeQueue m_uQueue;
        };
        class Database {
        private:

        public:
            Database(std::vector<Node> nodes);
            Database(Node node);
            Database();

            void recalculate();

            void save(std::string path);
            void load(std::string path);

            int m_nDataVersion;
            int m_nNodeSize;
            std::vector<Node> m_vNodes;
        };
        void setup();
    }
}

#pragma pack(pop)