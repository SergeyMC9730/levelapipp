#include "DCommand.h"
#include "DCommandEvent.h"
#include "appcommand.h"

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

void DCommand::run(DCommandEvent *event) {
    _callback(this, event);
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