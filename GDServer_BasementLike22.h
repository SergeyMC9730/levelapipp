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

#pragma once

#include "GDServer_BasementLike21.h"

namespace LevelAPI {
    namespace Backend {
        /*
          this GDPS has custom core that protects most requests from
          hacker attacks. own class has been made to take in a mind.
        */
        class GDServer_BasementLike22 : public GDServer_BasementLike21 {
        protected:
            std::vector<CURLParameter *> _setupGJLevelsArgs(int type, std::string str, int page) override;
            CURLConnection *_setupCURL(std::optional<CurlProxy> proxy, std::string secret) override;
        public:
            GDServer_BasementLike22(std::string endpoint);
            GDServer_BasementLike22(std::string endpoint, std::vector<LevelRange> list);
            GDServer_BasementLike22(std::string endpoint, LevelRangeList list);

            std::string getServerIdentifier() override;

            int getGameVersion() override;
        };
    }
}