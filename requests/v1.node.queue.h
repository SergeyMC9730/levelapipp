#pragma once

#include "APIRequest.h"

#include <memory>

using namespace httpserver;

namespace LevelAPI {
    namespace v1 {
        class QueueAddRequest : public APIRequest {
        public:
            std::shared_ptr<http_response> render(const http_request&);

            QueueAddRequest();
        };
    }
}