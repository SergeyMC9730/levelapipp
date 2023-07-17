#include "v1.img.request.h"

#include "../lapi_database.h"
#include <string>
#include <sys/stat.h>
#include <iostream>

#include "HTTPContentTypeImage.h"

#define file_exists(cstr) (stat(cstr, &buffer) == 0)

LevelAPI::v1::IMGRequest::IMGRequest() {
    this->request_name = "request data from server (OUTDATED)";
    this->request_url = "/api/v1/img/request/{file}";
}

std::shared_ptr<http_response> LevelAPI::v1::IMGRequest::render(const http_request &req) {
    auto file = std::string("images/") + std::string(req.get_arg("file"));
    //std::cout << file << std::endl;

    if (file.find("../") != std::string::npos || 
        file.find("/")   == 0                 || 
        file.find("~")   != std::string::npos ||  
      !(file.find(".png") != std::string::npos))
    {
        return generateResponse("404 File Not Found", 404);
    }

    return sendFile(std::string(file), HTTPContentTypeImage());
}