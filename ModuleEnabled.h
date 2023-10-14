#pragma  once

#include <string>
#include <vector>

namespace LevelAPI {
    namespace Backend {
        class ModuleEnabled {
        public:
            virtual std::vector<std::string> getModules();
        };
    }
}