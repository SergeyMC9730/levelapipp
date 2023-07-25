#include "DCChoice.h"
#include "appcommand.h"
#include <stdexcept>
#include <variant>

DCChoice::DCChoice(std::string name, std::variant<std::string, int, double> value) {
    _name = name;
    _value = value;
}

dpp::command_option_choice DCChoice::generateChoice() {
    if (std::holds_alternative<std::string>(_value)) {
        dpp::command_option_choice ch(_name, std::get<std::string>(_value));
    
        return ch;
    } else if (std::holds_alternative<int>(_value)) {
        dpp::command_option_choice ch(_name, (int64_t)std::get<int>(_value));
    
        return ch;
    } else if (std::holds_alternative<double>(_value)) {
        dpp::command_option_choice ch(_name, std::get<double>(_value));
    
        return ch;
    }

    throw std::runtime_error("invalid choice type");
}