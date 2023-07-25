#pragma once

#include "dispatcher.h"
#include <dpp.h>

#include <string>

#include <memory>

class DCommandEvent {
public:
    dpp::slashcommand_t _event;
public:
    DCommandEvent(const dpp::slashcommand_t &event);

    template<typename T>
    const T getArgument(std::string name);

    const dpp::slashcommand_t getEvent();
};