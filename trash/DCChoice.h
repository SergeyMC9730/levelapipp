#pragma once

#include <string>
#include <variant>

#include <dpp.h>

class DCChoice {
public:
    std::string _name;
    std::variant<std::string, int, double> _value;

    DCChoice(std::string name, std::variant<std::string, int, double> value);

    dpp::command_option_choice generateChoice();
};