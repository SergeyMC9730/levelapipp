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
	// how command has been executed (using /command, [button] or _select menu_
        enum DCommandEventType {
	    // command has been executed by using /command
            ET_Command = 0,
            // command has been executed by using [button]
            ET_Button = 1,
            // command has been executed by using select menu popup
            ET_SelectMenu = 2
        };

	// frontend for DPP command
        class DCommand {
        public:
            using DCommandCallback = std::function<void(DCommand *, DCommandEvent *event)>;

	    // linked cluster
            dpp::cluster *_cluster;
        protected:
	    // instance of the command
            dpp::slashcommand _instance;

	    // all logic happens inside this callback.
	    // just a unified callback for three events described in DCommandEventType.
            DCommandCallback _callback;

	    // discord bot id
            dpp::snowflake _botID;

            // returns empty callback by default.
	    // class that inherits DCommand can replace empty function with own data
            virtual DCommandCallback getDefaultCallback();

            // available events
            std::vector<std::pair<std::string, DCommandEventType>> _events;

            // all event logic happens here
            virtual void eventCallback(std::string name, DCommandEvent *event);

            // registers specific event for this command.
	    // it's useful when adding buttons and popup menues for this DCommand
            void registerEvent(std::string name, DCommandEventType type);
        public:
            DCommand(dpp::snowflake botID, DCommandCallback callback);
            ~DCommand();

	    // get _instance
            const dpp::slashcommand getCommand();

	    // run _callback with slashcommand
            virtual void run(const dpp::slashcommand_t &event);
            // run _callback with button_click
            virtual void run(const dpp::button_click_t &event);
	    // run _callback with select_click
            virtual void run(const dpp::select_click_t &event);

	    // get name of this DCommand
            virtual std::string getCommandName();

	    // get all registered by DCommand events
            const std::vector<std::pair<std::string, DCommandEventType>> getRegistedEvents();
        };
    }
}
