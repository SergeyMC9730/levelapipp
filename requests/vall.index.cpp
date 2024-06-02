/**
 *  LevelAPI - Geometry Dash level cacher with search functionality and more.
    Copyright (C) 2024  Sergei Baigerov

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

#include "vall.index.h"

#include "../lapi_database.h"
#include "../HttpController.h"
#include "HTTPContentTypeHTML.h"

LevelAPI::v1::IndexRequest::IndexRequest() {
    this->request_name = "index page";
    this->request_url = "/";
}

std::shared_ptr<http_response> LevelAPI::v1::IndexRequest::render(const http_request &req) {
    auto nodes = DatabaseController::database->m_vNodes;

    std::vector<std::string> node_names = {};

    for (auto node : nodes) {
        node_names.push_back(node->m_sInternalName);
    }

    return generateResponse(HttpController::generateIndex(node_names), HTTPContentTypeHTML());
}
