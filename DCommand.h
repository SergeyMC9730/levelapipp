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

#include "DiscordInstance.h"
#include "appcommand.h"
#include "cluster.h"
#include "snowflake.h"
#include <dpp.h>

#include <functional>

#include "DCommandEvent.h"

namespace LevelAPI {
    namespace Frontend {
        enum DCommandEventType {
            ET_Command = 0,
            ET_Button = 1,
            ET_SelectMenu = 2
        };

        class DCommand {
        public:
            using DCommandCallback = std::function<void(DCommand *, DCommandEvent *event)>;

            dpp::cluster *_cluster;
        protected:

            dpp::slashcommand _instance;

            DCommandCallback _callback;

            dpp::snowflake _botID;

            virtual DCommandCallback getDefaultCallback();

            std::vector<std::pair<std::string, DCommandEventType>> _events;

            virtual void eventCallback(std::string name, DCommandEvent *event);

            void registerEvent(std::string name, DCommandEventType type);
        public:
            DCommand(dpp::snowflake botID, DCommandCallback callback);
            ~DCommand();

            const dpp::slashcommand getCommand();
            
            virtual void run(const dpp::slashcommand_t &event);
            virtual void run(const dpp::button_click_t &event);
            virtual void run(const dpp::select_click_t &event);

            virtual std::string getCommandName();

            const std::vector<std::pair<std::string, DCommandEventType>> getRegistedEvents();
        };
    }
}