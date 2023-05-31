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

#define GET_KEY_GKSTRING(container, keyN, member) \
    if (container->variableExists(keyN)) member = std::string(std::get<std::string>(container->getVariable(keyN)).data());
#define GET_KEY_GKINT(container, keyN, member) \
    if (container->variableExists(keyN)) member = std::get<int>(container->getVariable(keyN));
#define GET_KEY_GKBOOL(container, keyN, member) \
    if (container->variableExists(keyN)) member = std::get<bool>(container->getVariable(keyN));
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
    virtual std::variant<std::string, int, float, bool> getVariable(int id);

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