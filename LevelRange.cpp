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

#include "LevelRange.h"
#include <stdexcept>

using namespace LevelAPI;

LevelRange::LevelRange(int min, int max, std::string ver) {
    m_nMin = min;
    m_nMax = max;
    m_sGDVer = ver;
}

int LevelRange::getLevelsInRange() {
    return m_nMax - m_nMin;
}

int LevelRange::getGV() {
    float gv = std::stof(m_sGDVer);
    return (int)(gv * 10.f);
}