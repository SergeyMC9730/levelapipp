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

#pragma once

#include <unordered_map>
#include <string>
#include <functional>
#include <variant>
#include <vector>

#define GKSTRING 0
#define GKINT 1
#define GKBOOL 2
#define GKBASE64 3
#define GKFLOAT 4

#define GET_KEY_GKSTRING(container, keyN, member) \
    if (container->variableExists(keyN)) member = container->getVariable<std::string>(keyN);
#define GET_KEY_GKINT(container, keyN, member) \
    if (container->variableExists(keyN)) member = container->getVariable<int>(keyN);
#define GET_KEY_GKBOOL(container, keyN, member) \
    if (container->variableExists(keyN)) member = container->getVariable<bool>(keyN);
#define GET_KEY_GKFLOAT(container, keyN, member) \
    if (container->variableExists(keyN)) member = container->getVariable<float>(keyN);
#define GET_KEY_GKBASE64(container, keyN, member) GET_KEY_GKSTRING(container, keyN, member);
#define GET_KEY(container, keyN, member, vType) GET_KEY_##vType(container, keyN, member)

class RobTopStringContainer {
protected:
    std::string m_sOriginalString;

    std::unordered_map<int, std::variant<std::string, int, float, bool>> m_mContainer;
    std::unordered_map<int, std::variant<
        std::function<std::variant<std::string, int, float, bool>(std::string inputString, int inputID)>,
        std::function<std::variant<std::string, int, float, bool>(std::string inputString, int inputID, int custom_variable)>
    >> m_mFunctionContainer;
    std::unordered_map<int, int> m_mFuncCustomArgList;

    virtual std::string variantToString(std::variant<std::string, int, float, bool> var);
public:
    // virtual std::variant<std::string, int, float, bool> getVariable(int id);
    template<typename T = int>
    T getVariable(int id);

    virtual void setParserForVariable(int index, std::function<std::variant<std::string, int, float, bool>(std::string inputString, int inputID)> func);
    virtual void setParserForVariable(std::vector<int> indexes, std::function<std::variant<std::string, int, float, bool>(std::string inputString, int inputID)> func);
    virtual void setParserForVariable(int index, std::function<std::variant<std::string, int, float, bool>(std::string inputString, int inputID, int customArgumentValue)> func, int carg);
    virtual void setParserForVariable(std::vector<int> indexes, std::function<std::variant<std::string, int, float, bool>(std::string inputString, int inputID, int customArgumentValue)> func, int carg);

    virtual void parse();

    virtual bool variableExists(int id);

    virtual void setString(std::string str);

    RobTopStringContainer(std::string str);
    RobTopStringContainer();
};