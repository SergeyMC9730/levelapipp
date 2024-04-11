#pragma  once

#include <string>
#include <vector>

namespace LevelAPI {
    namespace Backend {
        class ModuleEnabled {
        public:
            // get loaded modules for class that inherits ModuleEnabled
            virtual std::vector<std::string> getModules();
        };
    }
}
