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

#include "SmallLevel.h"

using namespace LevelAPI::Backend;

SmallLevel::SmallLevel() {}
SmallLevel::SmallLevel(std::string a, std::string b, int c) {
    this->m_sLevelName = a;
    this->m_nLevelID = c;
    this->m_sAuthor = b;
}

bool SmallLevel::operator==(const SmallLevel& rhs) {
    if(rhs.m_nLevelID == this->m_nLevelID && rhs.m_sLevelName == this->m_sLevelName && rhs.m_sAuthor == this->m_sAuthor) return true;
    return false;
}


bool SmallLevel::operator==(const SmallLevel *rhs) {
    if(rhs->m_nLevelID == this->m_nLevelID && rhs->m_sLevelName == this->m_sLevelName && rhs->m_sAuthor == this->m_sAuthor) return true;
    return false;
}