#pragma once

#include "Account19.h"

class Account21 : public Account19 {
protected:
    // add parsers for our KV container
    void addParsers() override;
public:
    // moderator level: Standard, Elder
    int modLevel;

    // parse userdata from string
    void parseFromString(std::string server_response) override;
};
