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

#include "DCommand.h"
#include "DCommandEvent.h"
#include "appcommand.h"
#include <utility>

using namespace LevelAPI::Frontend;

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