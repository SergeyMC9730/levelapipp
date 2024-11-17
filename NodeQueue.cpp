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

#include "lapi_database.h"
#include <vector>

#include "Tools.h"

using namespace LevelAPI::DatabaseController;

NodeQueue::NodeQueue(const NodeCommandQueue &q, bool executeQueue, int runtimeState) {
    m_vCommandQueue.push_back(q);
    m_bExecuteQueue = executeQueue;
    m_nRuntimeState = runtimeState;
    currentState = NC_IDLE;
    
    setupJSON();

    _jsonObject["commandQueue"].push_back(q);

    _jsonObject["executeQueue"] = executeQueue;
    _jsonObject["runtimeState"] = runtimeState;
}
NodeQueue::NodeQueue(const std::list<NodeCommandQueue> &vec, bool executeQueue, int runtimeState) {
    m_vCommandQueue = vec;
    m_bExecuteQueue = executeQueue;
    m_nRuntimeState = runtimeState;
    currentState = NC_IDLE;

    setupJSON();

    _jsonObject["commandQueue"] = nlohmann::json::array();

    for (const NodeCommandQueue &q : m_vCommandQueue) {
        _jsonObject["commandQueue"].push_back(q);
    }
    _jsonObject["executeQueue"] = executeQueue;
    _jsonObject["runtimeState"] = runtimeState;
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
    _jsonObject["commandQueue"].clear();
    _jsonObject["executeQueue"] = m_bExecuteQueue;
    _jsonObject["runtimeState"] = m_nRuntimeState;
    _jsonObject["resolverQueue"] = m_vResolverQueuedLevels;

    // printf("rsize: %d %d\n", _jsonObject["resolverQueue"].size(), m_vResolverQueuedLevels.size());

    int i = 0;

    for (NodeCommandQueue &q : m_vCommandQueue) {
        q.save();

        _jsonObject["commandQueue"].push_back(q);
    }
}

void NodeQueue::recover() {
    GET_JSON_VALUE(_jsonObject, "executeQueue", m_bExecuteQueue, bool);
    GET_JSON_VALUE(_jsonObject, "runtimeState", m_nRuntimeState, int);

    int i = 0;
    while(i < _jsonObject["commandQueue"].size()) {
        NodeCommandQueue cmd;
        cmd._jsonObject = _jsonObject["commandQueue"].at(i);
        cmd.recover();

        m_vCommandQueue.push_back(cmd);

        i++;
    }

    i = 0;
    while(i < _jsonObject["resolverQueue"].size()) {
        m_vResolverQueuedLevels.push_back(_jsonObject["resolverQueue"].at(i).get<int>());

        i++;
    }
}

void NodeQueue::setupJSON() {
    _jsonObject = nlohmann::json();
}

NodeQueue::~NodeQueue() {

}
