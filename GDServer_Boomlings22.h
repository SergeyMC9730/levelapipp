#include "GDServer_BoomlingsLike22.h"

namespace LevelAPI {
    namespace Backend {
        class GDServer_Boomlings22 : public GDServer_BoomlingsLike22 {
        public:
            GDServer_Boomlings22(std::string endpoint);

            std::string getServerName() override;
            std::string getServerIdentifier() override;
        };
    }
}