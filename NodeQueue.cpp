#include "lapi_database.h"
#include <vector>

using namespace LevelAPI::DatabaseController;

NodeQueue::NodeQueue(NodeCommandQueue q, bool executeQueue, int runtimeState) {
    m_vCommandQueue = std::vector<NodeCommandQueue>();
    m_vCommandQueue.push_back(q);
    m_bExecuteQueue = executeQueue;
    m_nRuntimeState = runtimeState;
}
NodeQueue::NodeQueue(std::vector<NodeCommandQueue> vec, bool executeQueue, int runtimeState) {
    m_vCommandQueue = vec;
    m_bExecuteQueue = executeQueue;
    m_nRuntimeState = runtimeState;
}
NodeQueue::NodeQueue(bool executeQueue, int runtimeState) {
    m_vCommandQueue = std::vector<NodeCommandQueue>();
    m_bExecuteQueue = executeQueue;
    m_nRuntimeState = runtimeState;
}
NodeQueue::NodeQueue() {
    NodeQueue(false, 0);
}