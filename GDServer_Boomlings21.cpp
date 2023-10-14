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

#include "GDServer_Boomlings21.h"
#include "Translation.h"

LevelAPI::Backend::GDServer_Boomlings21::GDServer_Boomlings21(std::string endpoint) : GDServer_BoomlingsBase(), GDServer_BoomlingsLike21(endpoint, getRanges()) {}

std::string LevelAPI::Backend::GDServer_Boomlings21::getServerName() {
    return Frontend::Translation::getByKey("lapi.gdserver_boomlings.name");
}

std::string LevelAPI::Backend::GDServer_Boomlings21::getServerIdentifier() {
    return "gdserver_boomlings";
}

std::vector<std::string> LevelAPI::Backend::GDServer_Boomlings21::getModules() {
    return {"gdhistory"};
}