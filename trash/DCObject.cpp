#include "DCObject.h"
#include "DCChoice.h"
#include "DCObjectType.h"
#include <stdexcept>
#include <variant>

void DCObject::parse() {
    if (_objects.count("name") > 0) {
        auto name = _objects["name"];

        if (std::holds_alternative<std::string>(name)) {
            _name = std::get<std::string>(name);
        } else {
            throw std::runtime_error("\"name\" should be a string.");
        }
    } else {
        throw std::runtime_error("\"name\" is required element in DCObject, but it was not found in the object map.");
    }

    if (_objects.count("description") > 0) {
        auto description = _objects["name"];

        if (std::holds_alternative<std::string>(description)) {
            _description = std::get<std::string>(description);
        } else {
            throw std::runtime_error("\"description\" should be a string.");
        }
    } else {
        throw std::runtime_error("\"description\" is required element in DCObject, but it was not found in the object map.");
    }

    if (_objects.count("options") > 0) {
        auto options = _objects["options"];

        if (std::holds_alternative<std::vector<DCObject>>(options)) {
            _options = std::get<std::vector<DCObject>>(options);
        } else {
            throw std::runtime_error("\"options\" should be an array of DC _objects.");
        }
    }

    if (_objects.count("required") > 0) {
        auto required = _objects["required"];

        if (std::holds_alternative<std::vector<DCObject>>(required)) {
            _required = std::get<bool>(required);
        } else {
            throw std::runtime_error("\"required\" should be a boolean.");
        }
    }

    if (_objects.count("type") > 0) {
        // assumed to be an command argument
        auto type = _objects["type"];

        if (std::holds_alternative<DCObjectType>(type)) {
            _type = std::get<DCObjectType>(type);
        } else {
            throw std::runtime_error("\"type\" should be an enum.");
        }

        // check type

        if (_type == DCObjectType::DCSubCommandGroup) {
            // check options if they exists
            int i = 0;
            while (i < _options.size()) {
                auto object = _options[i];

                if (object._type != DCObjectType::DCSubCommand) {
                    throw std::runtime_error("Child of Subcommand group is not sub command.");
                }

                i++;
            }
        }
    } else {
        _type = DCommand;
        
        // its a command
        // check arguments

        int i = 0;
            while (i < _options.size()) {
                auto object = _options[i];

                if (object._type == DCObjectType::DCommand) {
                    throw std::runtime_error("Child of Command cannot be a Command");
                }

                i++;
            }
    }

    if (_objects.count("choices") > 0) {
        if (_type != DCObjectType::DCString && _type != DCObjectType::DCInteger && _type != DCObjectType::DCNumber) {
            throw std::runtime_error("There is no reason to put choise list for this element");
        }

        auto choices = _objects["choices"];

        if (std::holds_alternative<std::vector<DCChoice>>(choices)) {
            _choices = std::get<std::vector<DCChoice>>(choices);
        } else {
            throw std::runtime_error("\"choices\" should be an array of DC choices.");
        }

        int i = 0;

        while (i < _choices.size()) {
            auto choise = _choices[i];

            if (std::holds_alternative<std::string>(choise._value) && (_type == DCNumber)) {
                throw std::runtime_error("invalid choice: expected number, got string");
            }

            if (std::holds_alternative<std::string>(choise._value) && (_type == DCInteger)) {
                throw std::runtime_error("invalid choice: expected integer, got string");
            }

            if (std::holds_alternative<int>(choise._value) && (_type == DCNumber)) {
                throw std::runtime_error("invalid choice: expected number, got integer");
            }

            if (std::holds_alternative<double>(choise._value) && (_type == DCInteger)) {
                throw std::runtime_error("invalid choice: expected integer, got number");
            }
            
            if (std::holds_alternative<int>(choise._value) && (_type == DCString)) {
                throw std::runtime_error("invalid choice: expected string, got integer");
            }

            if (std::holds_alternative<double>(choise._value) && (_type == DCString)) {
                throw std::runtime_error("invalid choice: expected string, got integer");
            }

            i++;
        }
    }
}

DCObject::DCObject(DCObjectMapping objects) {
    _objects = objects;

    parse();
    
    return;
}
DCObject::~DCObject() {}

std::string DCObject::getName() {
    return _name;
}