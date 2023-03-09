#include "lapi_database.h"

using namespace LevelAPI::DatabaseController;

NodeCommandQueue::NodeCommandQueue(int command, std::string text) {
    m_nCommand = command;
    m_sText = text;
}

NodeCommandQueue::NodeCommandQueue() {
    NodeCommandQueue(0, "-");
}