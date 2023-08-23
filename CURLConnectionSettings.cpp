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

#include "curl_backend.h"

using namespace LevelAPI::Backend;

CURLConnectionSettings::CURLConnectionSettings(FILE *file) {
    m_bWriteToMemory = false;
    m_pFile = file;
    m_nSize = 0;
}
CURLConnectionSettings::CURLConnectionSettings(void *memory_pointer, int maxSize) {
    m_bWriteToMemory = true;
    m_nMaxMemorySize = maxSize;
    m_pData = memory_pointer;
    m_nSize = 0;
}
CURLConnectionSettings::~CURLConnectionSettings() {}