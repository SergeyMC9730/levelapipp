#pragma once

#include "api_request.h"

#include <memory>

using namespace httpserver;

namespace LevelAPI {
    namespace v1 {
        class StatsRequest : public APIRequest {
        public:
            std::shared_ptr<http_response> render(const http_request&);

            StatsRequest();
        };
    }
}