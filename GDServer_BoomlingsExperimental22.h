#include "GDServer_BoomlingsLike22.h"

namespace LevelAPI {
    namespace Backend {
        class GDServer_BoomlingsExperimental22 : public GDServer_BoomlingsLike22 {
        public:
            GDServer_BoomlingsExperimental22(std::string endpoint);

            std::string getServerName() override;
            std::string getServerIdentifier() override;
        };
    }
}