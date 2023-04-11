#pragma once

#include <memory>
#include <string>

namespace LevelAPI {
    namespace Backend {
        class SmallLevel {
        public:
            std::string m_sLevelName;
            std::string m_sAuthor;
            int m_nLevelID;

            SmallLevel();
            SmallLevel(std::string ln, std::string a, int id);

            bool operator==(const SmallLevel& rhs);
            bool operator==(const SmallLevel *rhs);
        };
    }
}