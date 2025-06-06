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

void RobTopStringContainer::setParserForVariable(int index, ParseFunc func) {
    m_mFunctionContainer[index] = func;
}
void RobTopStringContainer::setParserForVariable(const std::vector<int> &indexes, ParseFunc func) {
    int i = 0;
    while(i < indexes.size()) {
        setParserForVariable(indexes[i], func);
        i++;
    }
}
void RobTopStringContainer::setParserForVariable(int index, ParseFuncCustom func, int carg) {
    m_mFunctionContainer[index] = func;
    m_mFuncCustomArgList[index] = carg;
}
void RobTopStringContainer::setParserForVariable(const std::vector<int> &indexes, ParseFuncCustom func, int carg) {
    int i = 0;
    while(i < indexes.size()) {
        setParserForVariable(indexes[i], func, carg);
        i++;
    }
}

void RobTopStringContainer::resetValues() {
    m_mContainer = {};
}

std::string RobTopStringContainer::variantToString(ParsableTypes var) {
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

bool RobTopStringContainer::variableExists(int id) const {
    return m_mContainer.count(id);
}

void RobTopStringContainer::setString(std::string str) {
    this->m_sOriginalString = str;
}

void RobTopStringContainer::parse() {
    int currentKey = 0;
    int i = 0;
    auto splittedString = splitString(m_sOriginalString.c_str(), m_sObjectSeparator);

    while(i < splittedString.size()) {
        try {
            currentKey = std::stoi(splittedString[i]);
        } catch (std::invalid_argument &e) { return; }

        i++;

        if(m_mFunctionContainer.count(currentKey)) {
            if(std::holds_alternative<ParseFunc>(m_mFunctionContainer[currentKey])) {
                auto func = std::get<ParseFunc>(m_mFunctionContainer[currentKey]);
                auto res = func(splittedString[i], currentKey, this);
                m_mContainer[currentKey] = res;
            }
            if(std::holds_alternative<ParseFuncCustom>(m_mFunctionContainer[currentKey])) {
                auto func = std::get<ParseFuncCustom>(m_mFunctionContainer[currentKey]);
                if(m_mFuncCustomArgList.count(currentKey)) {
                    auto res = func(splittedString[i], currentKey, m_mFuncCustomArgList[currentKey], this);
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

void RobTopStringContainer::setObjectSeparator(char sep) {
    m_sObjectSeparator = sep;
}
void RobTopStringContainer::setArrayEntrySeparator(char sep) {
    m_sArrayEntrySeparator = sep;
}

char RobTopStringContainer::getObjectSeparator() const {
    return m_sObjectSeparator;
}
char RobTopStringContainer::getArrayEntrySeparator() const {
    return m_sArrayEntrySeparator;
}
