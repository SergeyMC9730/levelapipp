#pragma once

#include "dispatcher.h"
#include <dpp.h>

#include <string>

#include <memory>

#include <variant>

class DCommandEvent {
public:
    std::variant<dpp::slashcommand_t, dpp::button_click_t, dpp::select_click_t> _event;
public:
    DCommandEvent(const dpp::slashcommand_t &event);
    DCommandEvent(const dpp::button_click_t &event);
    DCommandEvent(const dpp::select_click_t &event);

    template<typename T>
    const T getArgument(std::string name);

    bool argumentExists(std::string name);

    const dpp::slashcommand_t getCommandEvent();
    const dpp::button_click_t getButtonEvent();
    const dpp::select_click_t getSelectMenuEvent();

    bool isCommand();
    bool isButton();
    bool isSelectMenu();
};