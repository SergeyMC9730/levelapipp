#pragma once

#include "APIRequest.h"

#include <memory>

using namespace httpserver;

namespace LevelAPI {
    namespace v1 {
        class LevelDownloadRequest : public APIRequest {
        public:
            std::shared_ptr<http_response> render(const http_request&);

            LevelDownloadRequest();
        };
    }
}