#include "lapi_database.h"
#include "json/single_include/nlohmann/json.hpp"
#include "Tools.h"

using namespace LevelAPI::DatabaseController;

NodeCommandQueue::NodeCommandQueue(int command, std::string text) {
    m_nCommand = command;
    m_sText = text;

    setupJSON();

    save();
}

NodeCommandQueue::NodeCommandQueue() {
    m_nCommand = NC_NONE;
    m_sText = "-";

    setupJSON();

    save();
}

void NodeCommandQueue::save() {
    _jsonObject["type"] = m_nCommand;
    _jsonObject["data"] = m_sText;
}

void NodeCommandQueue::recover() {
    GET_JSON_VALUE(_jsonObject, "type", m_nCommand, int);
    GET_JSON_VALUE(_jsonObject, "data", m_sText, std::string);
}

void NodeCommandQueue::setupJSON() {
    _jsonObject = nlohmann::json();
}

NodeCommandQueue::~NodeCommandQueue() {}