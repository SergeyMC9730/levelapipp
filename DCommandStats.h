#pragma once

#include "DCommand.h"

class DCommandStats : public DCommand {
protected:
    DCommandCallback getDefaultCallback() override;
public:
    DCommandStats(dpp::snowflake botID);
    ~DCommandStats();

    std::string getCommandName() override;
};