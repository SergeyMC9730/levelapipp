#pragma once

#include <string>

namespace LevelAPI {
    namespace DatabaseController {
        class LevelRelease {
        public:
            ~LevelRelease();

            int m_nGameVersion;
            std::string m_fActualVersion;
        };
    }
}