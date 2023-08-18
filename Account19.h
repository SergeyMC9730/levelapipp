#pragma once

#include "Account10.h"

class Account19 : public Account10 {
protected:
    // add parsers for our KV container
    void addParsers() override;
public:
    // account ID
    int accountID;

    // parse userdata from string
    void parseFromString(std::string server_response) override;
};
