#pragma once

#include "curl_backend.h"

#include <string>

#include "gmd2pp/level-converter/GJGameLevel.h"

namespace LevelAPI {
    namespace Backend {
        class GDServer {
        protected:
            std::string m_sEndpointURL;
            CURLConnection *m_pLinkedCURL;
        public:
            GDServer();
            ~GDServer();

            virtual GJGameLevel *getLevelMetaByID(int id);
        };
    }
}