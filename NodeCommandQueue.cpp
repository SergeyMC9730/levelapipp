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
    commandJson["type"] = m_nCommand;
    commandJson["data"] = m_sText;
}

void NodeCommandQueue::recover() {
    GET_JSON_VALUE(commandJson, "type", m_nCommand, int);
    GET_JSON_VALUE(commandJson, "data", m_sText, std::string);
}

void NodeCommandQueue::setupJSON() {
    commandJson = nlohmann::json();
}

NodeCommandQueue::~NodeCommandQueue() {}