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

#include "GDServer_BasementGDPS.h"

#include "LevelRangeList.h"
#include "Translation.h"

LevelAPI::Backend::GDServer_BasementGDPS::GDServer_BasementGDPS(std::string endpoint) : GDServer_BasementLike22(endpoint, (LevelRangeList){
    {
        {1, 999999, "2.2"}
    }
}) {}

std::string LevelAPI::Backend::GDServer_BasementGDPS::getServerName() {
    return Frontend::Translation::getByKey("lapi.gdserver_basementgdps.name");
}
std::string LevelAPI::Backend::GDServer_BasementGDPS::getServerIdentifier() {
    return "gdserver_basementGDPS";
}