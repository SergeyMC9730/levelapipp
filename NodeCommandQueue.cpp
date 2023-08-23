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