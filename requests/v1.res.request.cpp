#include "v1.res.request.h"

#include "../lapi_database.h"
#include <string>
#include <sys/stat.h>
#include <iostream>

#include "HTTPContentTypeImage.h"
#include "HTTPContentTypeVideoMP4.h"
#include "HTTPContentTypeAudio.h"
#include "HTTPContentTypeText.h"

#define file_exists(cstr) (stat(cstr, &buffer) == 0)

LevelAPI::v1::ResourceRequest::ResourceRequest() {
    this->request_name = "request data from server";
    this->request_url = "/api/v1/res/request/{file}";
}

std::shared_ptr<http_response> LevelAPI::v1::ResourceRequest::render(const http_request &req) {
    auto file = std::string("resources/") + std::string(req.get_arg("file"));
    //std::cout << file << std::endl;

    if (file.find("../") != std::string::npos || 
        file.find("/")   == 0                 || 
        file.find("~")   != std::string::npos  )
    {
        return sendFile("resources/auto.png", HTTPContentTypeImage());
    }

    if (file.find(".mp3") != std::string::npos) {
        return sendFile(std::string(file), HTTPContentTypeAudio());
    } else if (file.find(".mp4") != std::string::npos) {
        return sendFile(std::string(file), HTTPContentTypeVideoMP4());
    } else if (file.find(".png") != std::string::npos) {
        return sendFile(std::string(file), HTTPContentTypeImage());
    } else {
        return sendFile(std::string(file), HTTPContentTypeText());
    }
}