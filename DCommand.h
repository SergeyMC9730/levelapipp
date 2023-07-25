#pragma once

#include "appcommand.h"
#include "snowflake.h"
#include <dpp.h>

#include <functional>

#include "DCommandEvent.h"

class DCommand {
public:
    using DCommandCallback = std::function<void(DCommand *, DCommandEvent *event)>;
protected:
    dpp::slashcommand _instance;

    DCommandCallback _callback;

    dpp::snowflake _botID;

    virtual DCommandCallback getDefaultCallback();
public:
    DCommand(dpp::snowflake botID, DCommandCallback callback);
    ~DCommand();

    const dpp::slashcommand getCommand();
    
    virtual void run(const dpp::slashcommand_t &event);
    virtual void run(DCommandEvent *event);

    virtual std::string getCommandName();
};