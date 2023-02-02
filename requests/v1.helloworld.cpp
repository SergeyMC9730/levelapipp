#include "v1.helloworld.h"

LevelAPI::v1::HelloWorldRequest::HelloWorldRequest() {
    this->request_name = "hello world";
    this->request_url = "/api/v1/hello";
}

std::shared_ptr<http_response> LevelAPI::v1::HelloWorldRequest::render(const http_request &req) {
    auto a = req.get_arg("mykey");
    string_response b = string_response(std::string(a.data()));
    SEND_RESPONSE(b);
}