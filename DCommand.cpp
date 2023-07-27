#include "DCommand.h"
#include "DCommandEvent.h"
#include "appcommand.h"
#include <utility>

DCommand::DCommand(dpp::snowflake botID, DCommandCallback callback) {
    _callback = callback;
    _botID = botID;
}
DCommand::~DCommand() {}

void DCommand::run(const dpp::slashcommand_t &event) {
    DCommandEvent *_event = new DCommandEvent(event);

    _callback(this, _event);

    delete _event;
}
void DCommand::run(const dpp::button_click_t &event) {
    DCommandEvent *_event = new DCommandEvent(event);

    eventCallback(event.custom_id, _event);

    delete _event;
}
void DCommand::run(const dpp::select_click_t &event) {
    DCommandEvent *_event = new DCommandEvent(event);

    eventCallback(event.custom_id, _event);

    delete _event;
}

void DCommand::eventCallback(std::string name, DCommandEvent *event) {
    return;
}
void DCommand::registerEvent(std::string name, DCommandEventType type) {
    _events.push_back(std::make_pair(name, type));
}
const std::vector<std::pair<std::string, DCommandEventType>> DCommand::getRegistedEvents() {
    return _events;
}

DCommand::DCommandCallback DCommand::getDefaultCallback() {
    return [&](DCommand *cmd, DCommandEvent *event) {};
}

const dpp::slashcommand DCommand::getCommand() {
    return _instance;
}

std::string DCommand::getCommandName() {
    return "";
}