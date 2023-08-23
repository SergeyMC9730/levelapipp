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

#pragma once

#include "DCommand.h"

#include "Level.h"
#include "cluster.h"
#include "message.h"
#include "snowflake.h"

#include <unordered_map>

#include "SearchFilter.h"

namespace LevelAPI {
    namespace Frontend {
        enum SearchInstanceType {
            SearchList = 0,
            SearchID = 1
        };

        class SearchInstance : public LevelAPI::Backend::SearchFilter {
        public:
            dpp::snowflake _discordUserID;
            dpp::message _discordMessage;
            dpp::cluster *_discordCluster;
            SearchInstanceType _discordType;
            int _levelID;
            std::string _node;

            LevelAPI::Backend::SearchFilter *getParent();
            SearchInstance *getSelf();
        };

        class DCommandSearch : public DCommand {
        protected:
            DCommandCallback getDefaultCallback() override;
            void eventCallback(std::string name, DCommandEvent *event) override;
        public:
            std::unordered_map<dpp::snowflake, SearchInstance> _instances;
        public:
            dpp::message render(std::vector<LevelAPI::DatabaseController::Level *> levels, std::string node, int page = -1);

            DCommandSearch(dpp::snowflake botID);
            ~DCommandSearch();

            std::string getCommandName() override;
        };
    }
}