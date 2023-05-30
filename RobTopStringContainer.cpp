#include <stdexcept>
#include <iostream>
#include <variant>

#include "RobTopStringContainer.hpp"
#include "StringSplit.h"

RobTopStringContainer::RobTopStringContainer() {

}
RobTopStringContainer::RobTopStringContainer(std::string str) {
    m_sOriginalString = str;
}

void RobTopStringContainer::setParserForVariable(int index, std::function<std::variant<std::string_view, int, float, bool> (std::string_view, int)> func) {
    m_mFunctionContainer[index] = func;
}
void RobTopStringContainer::setParserForVariable(std::vector<int> indexes, std::function<std::variant<std::string_view, int, float, bool> (std::string_view, int)> func) {
    int i = 0;
    while(i < indexes.size()) {
        setParserForVariable(indexes[i], func);
        i++;
    }
}
void RobTopStringContainer::setParserForVariable(int index, std::function<std::variant<std::string_view, int, float, bool>(std::string_view inputString, int inputID, int customArgumentValue)> func, int carg) {
    m_mFunctionContainer[index] = func;
    m_mFuncCustomArgList[index] = carg;
}
void RobTopStringContainer::setParserForVariable(std::vector<int> indexes, std::function<std::variant<std::string_view, int, float, bool>(std::string_view inputString, int inputID, int customArgumentValue)> func, int carg) {
    int i = 0;
    while(i < indexes.size()) {
        setParserForVariable(indexes[i], func, carg);
        i++;
    }
}


std::variant<std::string_view, int, float, bool> RobTopStringContainer::getVariable(int id) {
    return m_mContainer[id];
}

std::string RobTopStringContainer::variantToString(std::variant<std::string_view, int, float, bool> var) {
    if (std::holds_alternative<std::string_view>(var)) {
        return std::string(std::get<std::string_view>(var));
    }
    if (std::holds_alternative<int>(var)) {
        return std::to_string(std::get<int>(var));
    }
    if (std::holds_alternative<float>(var)) {
        return std::to_string(std::get<float>(var));
    }
    if (std::holds_alternative<bool>(var)) {
        int v = static_cast<int>(std::get<bool>(var));
        return std::to_string(v);
    }
    return "";
}

bool RobTopStringContainer::variableExists(int id) {
    return m_mContainer.count(id);
}

void RobTopStringContainer::parse() {
    int currentKey = 0;
    int i = 0;
    auto splittedString = splitString(m_sOriginalString.c_str(), ':');

    while(i < splittedString.size()) {
        try {
            currentKey = std::stoi(splittedString[i]);
        } catch (std::invalid_argument &e) {
            return;
        }

        i++;

        if(m_mFunctionContainer.count(currentKey)) {
            if(std::holds_alternative<std::function<std::variant<std::string_view, int, float, bool>(std::string_view inputString, int inputID)>>(m_mFunctionContainer[currentKey])) {
                auto func = std::get<std::function<std::variant<std::string_view, int, float, bool>(std::string_view inputString, int inputID)>>(m_mFunctionContainer[currentKey]);
                auto res = func(splittedString[i], currentKey);
                m_mContainer[currentKey] = res;
            }
            if(std::holds_alternative<std::function<std::variant<std::string_view, int, float, bool>(std::string_view inputString, int inputID, int carg)>>(m_mFunctionContainer[currentKey])) {
                auto func = std::get<std::function<std::variant<std::string_view, int, float, bool>(std::string_view inputString, int inputID, int carg)>>(m_mFunctionContainer[currentKey]);
                if(m_mFuncCustomArgList.count(currentKey)) {
                    auto res = func(splittedString[i], currentKey, m_mFuncCustomArgList[currentKey]);
                    m_mContainer[currentKey] = res;
                }
            }
        } else {
            std::cout << "WARN : key " << currentKey << " is not implemented!" << std::endl;
           
            m_mContainer[currentKey] = splittedString[i];
        }

        i++;
    }
}