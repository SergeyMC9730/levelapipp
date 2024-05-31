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
        // search type
        enum SearchInstanceType {
            // search lots of levels in paged mode
            SearchList = 0,
            // get level info by its id
            SearchID = 1
        };

        class SearchInstance : public LevelAPI::Backend::SearchFilter {
        public:
            // who requested search
            dpp::snowflake _discordUserID;

            // which message should be updated
            dpp::message _discordMessage;

            // linked cluster
            dpp::cluster *_discordCluster;

            // search type. it can be SearchList or SearchID
            SearchInstanceType _discordType;

            // level id if using SearchInstanceType::SearchID
            int _levelID;

            // levelapi node
            std::string _node;

            // gets parent of this SearchInstance. required for leaving from search menu
            LevelAPI::Backend::SearchFilter *getParent();

            // returns this
            SearchInstance *getSelf();
        };

        class DCommandSearch : public DCommand {
        protected:
            DCommandCallback getDefaultCallback() override;
            void eventCallback(std::string name, DCommandEvent *event) override;
        public:
            // all instanced made on runtime
            std::unordered_map<dpp::snowflake, SearchInstance> _instances;
        public:
            // render search menu. result depends on what user requested.
            // it can be card for a single level or a pager.
            dpp::message render(std::vector<LevelAPI::DatabaseController::Level *> levels, std::string node, int page = -1);

            DCommandSearch(dpp::snowflake botID);
            ~DCommandSearch();

	    // /search
            std::string getCommandName() override;
        };
    }
}
