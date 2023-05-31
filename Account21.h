#pragma once

#include "Account19.h"

class Account21 : public Account19 {
protected:
    void addParsers() override;
public:
    int modLevel;

    void parseFromString(std::string server_response) override;
};