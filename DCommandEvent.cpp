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

#include "DCommandEvent.h"
#include "dispatcher.h"
#include <memory>
#include <variant>

using namespace LevelAPI::Frontend;

DCommandEvent::DCommandEvent(const dpp::slashcommand_t &event) : _event(event) {}
DCommandEvent::DCommandEvent(const dpp::button_click_t &event) : _event(event) {}
DCommandEvent::DCommandEvent(const dpp::select_click_t &event) : _event(event) {}

bool DCommandEvent::isCommand() {
    return std::holds_alternative<dpp::slashcommand_t>(_event);
}
bool DCommandEvent::isButton() {
    return std::holds_alternative<dpp::button_click_t>(_event);
}
bool DCommandEvent::isSelectMenu() {
    return std::holds_alternative<dpp::select_click_t>(_event);
}

const dpp::slashcommand_t DCommandEvent::getCommandEvent() {
    return std::get<dpp::slashcommand_t>(_event);
}
const dpp::button_click_t DCommandEvent::getButtonEvent() {
    return std::get<dpp::button_click_t>(_event);
}
const dpp::select_click_t DCommandEvent::getSelectMenuEvent() {
    return std::get<dpp::select_click_t>(_event);
}

bool DCommandEvent::argumentExists(std::string name) {
    if (std::holds_alternative<dpp::slashcommand_t>( _event)) {
        auto val = std::get<dpp::slashcommand_t>(_event).get_parameter(name);

        return val.index() != 0;
    }

    return false;
}

template<> 
const std::string DCommandEvent::getArgument(std::string name) {
    if (std::holds_alternative<dpp::slashcommand_t>( _event)) {
        std::string val = std::get<std::string>(std::get<dpp::slashcommand_t>(_event).get_parameter(name));

        return val;
    }

    return "";
}

template<> 
const int64_t DCommandEvent::getArgument(std::string name) {
    if (std::holds_alternative<dpp::slashcommand_t>( _event)) {
        int64_t val = std::get<int64_t>(std::get<dpp::slashcommand_t>(_event).get_parameter(name));

        return val;
    }

    return 0;
}

template<> 
const int DCommandEvent::getArgument(std::string name) {
    if (std::holds_alternative<dpp::slashcommand_t>( _event)) {
        int val = (int)std::get<int64_t>(std::get<dpp::slashcommand_t>(_event).get_parameter(name));

        return val;
    }

    return 0;
}

template<> 
const double DCommandEvent::getArgument(std::string name) {
    if (std::holds_alternative<dpp::slashcommand_t>( _event)) {
        double val = std::get<double>(std::get<dpp::slashcommand_t>(_event).get_parameter(name));

        return val;
    }

    return 0;
}