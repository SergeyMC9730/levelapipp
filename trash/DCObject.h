#pragma once

#include <unordered_map>
#include <string>
#include <variant>
#include <optional>
#include <vector>

#include "DCObjectType.h"
#include "DCChoice.h"

class DCObject {
public:
    using DCObjectMapping = std::unordered_map<std::string, std::variant<std::string, DCObjectType, bool, std::vector<DCObject>, std::vector<DCChoice>>>;
protected:
    std::string _name = "";
    std::string _description = "";
    int _type = 0;
    bool _required = true;
    std::vector<DCObject> _options = {};
    std::vector<DCChoice> _choices = {};

    DCObjectMapping _objects;

    void parse();
public:
    DCObject(DCObjectMapping objects);
    ~DCObject();

    std::string getName();


};