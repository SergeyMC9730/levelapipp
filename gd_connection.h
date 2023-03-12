#pragma once

#include "curl_backend.h"

#include <string>

namespace LevelAPI {
    namespace Backend {
        class GDServer : CURLConnection {
        private:
            std::string m_sEndpointURL;
        public:
            
        };
    }
}