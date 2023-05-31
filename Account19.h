#pragma once

#include "Account10.h"

class Account19 : public Account10 {
protected:
    void addParsers() override;
public:
    int accountID;

    void parseFromString(std::string server_response) override;
};