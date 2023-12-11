/**
 *  LevelAPI - Geometry Dash level cacher with search functionality and more.
    Copyright (C) 2023  Sergei Baigerov

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <https://www.gnu.org/licenses/>.
*/

#include <cstdint>
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

void RobTopStringContainer::setParserForVariable(int index, std::function<std::variant<std::string, int, float, bool> (std::string, int)> func) {
    m_mFunctionContainer[index] = func;
}
void RobTopStringContainer::setParserForVariable(std::vector<int> indexes, std::function<std::variant<std::string, int, float, bool> (std::string, int)> func) {
    int i = 0;
    while(i < indexes.size()) {
        setParserForVariable(indexes[i], func);
        i++;
    }
}
void RobTopStringContainer::setParserForVariable(int index, std::function<std::variant<std::string, int, float, bool>(std::string inputString, int inputID, int customArgumentValue)> func, int carg) {
    m_mFunctionContainer[index] = func;
    m_mFuncCustomArgList[index] = carg;
}
void RobTopStringContainer::setParserForVariable(std::vector<int> indexes, std::function<std::variant<std::string, int, float, bool>(std::string inputString, int inputID, int customArgumentValue)> func, int carg) {
    int i = 0;
    while(i < indexes.size()) {
        setParserForVariable(indexes[i], func, carg);
        i++;
    }
}

// template implementations for basic robtop values
template<> int RobTopStringContainer::getVariable<int>(int id) {
    return std::get<int>(m_mContainer[id]);
}

template<> std::string RobTopStringContainer::getVariable<std::string>(int id) {
    return std::get<std::string>(m_mContainer[id]);
}

template<> float RobTopStringContainer::getVariable<float>(int id) {
    return std::get<float>(m_mContainer[id]);
}

template<> bool RobTopStringContainer::getVariable<bool>(int id) {
    return std::get<bool>(m_mContainer[id]);
}

// template implementations for all unsigned integers
template<> uint8_t RobTopStringContainer::getVariable<uint8_t>(int id) {
    return static_cast<uint8_t>(getVariable<int>(id));
}

template<> uint16_t RobTopStringContainer::getVariable<uint16_t>(int id) {
    return static_cast<uint16_t>(getVariable<int>(id));
}

template<> uint32_t RobTopStringContainer::getVariable<uint32_t>(int id) {
    return static_cast<uint32_t>(getVariable<int>(id));
}

template <> uint64_t RobTopStringContainer::getVariable<uint64_t>(int id) {
    return static_cast<uint64_t>(getVariable<int>(id));
}

// template implementations for all signed integers

template<> int8_t RobTopStringContainer::getVariable<int8_t>(int id) {
    return static_cast<int8_t>(getVariable<uint8_t>(id));
}

template<> int16_t RobTopStringContainer::getVariable<int16_t>(int id) {
    return static_cast<int16_t>(getVariable<uint16_t>(id));
}

template<> int64_t RobTopStringContainer::getVariable<int64_t>(int id) {
    return static_cast<int64_t>(getVariable<uint64_t>(id));
}

// additonal template implementations

template<> double RobTopStringContainer::getVariable<double>(int id) {
    return (double)(getVariable<float>(id));
}

void RobTopStringContainer::resetValues() {
    m_mContainer = {};
}

std::string RobTopStringContainer::variantToString(std::variant<std::string, int, float, bool> var) {
    if (std::holds_alternative<std::string>(var)) {
        return std::get<std::string>(var);
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

void RobTopStringContainer::setString(std::string str) {
    this->m_sOriginalString = str;
}

void RobTopStringContainer::parse() {
    int currentKey = 0;
    int i = 0;
    auto splittedString = splitString(m_sOriginalString.c_str(), ':');

    while(i < splittedString.size()) {
        try {
            currentKey = std::stoi(splittedString[i]);
        } catch (std::invalid_argument &e) { return; }

        i++;

        if(m_mFunctionContainer.count(currentKey)) {
            if(std::holds_alternative<std::function<std::variant<std::string, int, float, bool>(std::string inputString, int inputID)>>(m_mFunctionContainer[currentKey])) {
                auto func = std::get<std::function<std::variant<std::string, int, float, bool>(std::string inputString, int inputID)>>(m_mFunctionContainer[currentKey]);
                auto res = func(splittedString[i], currentKey);
                m_mContainer[currentKey] = res;
            }
            if(std::holds_alternative<std::function<std::variant<std::string, int, float, bool>(std::string inputString, int inputID, int carg)>>(m_mFunctionContainer[currentKey])) {
                auto func = std::get<std::function<std::variant<std::string, int, float, bool>(std::string inputString, int inputID, int carg)>>(m_mFunctionContainer[currentKey]);
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