#include "lapi_database.h"
#include <vector>

#include "Tools.h"

using namespace LevelAPI::DatabaseController;

NodeQueue::NodeQueue(NodeCommandQueue *q, bool executeQueue, int runtimeState) {
    m_vCommandQueue.push_back(q);
    m_bExecuteQueue = executeQueue;
    m_nRuntimeState = runtimeState;
    currentState = NC_IDLE;
    
    setupJSON();

    int i = 0;
    while(i < m_vCommandQueue.size()) {
        queueJson["commandQueue"].push_back(m_vCommandQueue.at(i)->commandJson);
        i++;
    }
    queueJson["executeQueue"] = executeQueue;
    queueJson["runtimeState"] = runtimeState;
}
NodeQueue::NodeQueue(std::vector<NodeCommandQueue *> vec, bool executeQueue, int runtimeState) {
    m_vCommandQueue = vec;
    m_bExecuteQueue = executeQueue;
    m_nRuntimeState = runtimeState;
    currentState = NC_IDLE;

    setupJSON();

    queueJson["commandQueue"] = nlohmann::json::array();

    int i = 0;
    while(i < m_vCommandQueue.size()) {
        queueJson["commandQueue"].push_back(m_vCommandQueue.at(i)->commandJson);
        i++;
    }
    queueJson["executeQueue"] = executeQueue;
    queueJson["runtimeState"] = runtimeState;
}
NodeQueue::NodeQueue(bool executeQueue, int runtimeState) {
    m_bExecuteQueue = executeQueue;
    m_nRuntimeState = runtimeState;
    currentState = NC_IDLE;

    setupJSON();

    save();
}
NodeQueue::NodeQueue() {
    m_bExecuteQueue = false;
    m_nRuntimeState = 0;
    currentState = NC_IDLE;

    setupJSON();

    save();
    // NodeQueue(false, 0);
}

void NodeQueue::save() {
    queueJson["commandQueue"] = nlohmann::json::array();
    queueJson["executeQueue"] = m_bExecuteQueue;
    queueJson["runtimeState"] = m_nRuntimeState;

    int i = 0;
    while(i < m_vCommandQueue.size()) {
        m_vCommandQueue.at(i)->save();
        queueJson["commandQueue"].push_back(m_vCommandQueue.at(i)->commandJson);
        i++;
    }
}

void NodeQueue::recover() {
    GET_JSON_VALUE(queueJson, "executeQueue", m_bExecuteQueue, bool);
    GET_JSON_VALUE(queueJson, "runtimeState", m_nRuntimeState, int);

    int i = 0;
    while(i < queueJson["commandQueue"].size()) {
        m_vCommandQueue.push_back(new NodeCommandQueue());
        m_vCommandQueue.at(i)->commandJson = queueJson["commandQueue"].at(i);
        m_vCommandQueue.at(i)->recover();
        i++;
    }
}

void NodeQueue::setupJSON() {
    queueJson = nlohmann::json();
}

NodeQueue::~NodeQueue() {
    int i = 0;
    while(i < m_vCommandQueue.size()) {
        delete m_vCommandQueue.at(i);
        i++;
    }

}