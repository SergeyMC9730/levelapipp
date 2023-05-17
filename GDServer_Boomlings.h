#include "GDServer_BoomlingsLike21.h"

namespace LevelAPI {
    namespace Backend {
        class GDServer_Boomlings : public GDServer_BoomlingsLike21 {
        public:
            GDServer_Boomlings(std::string endpoint);

            std::string getServerName() override;
            std::string getServerIdentifier() override;
        };
    }
}