#include <unordered_map>
#include <string>
#include <functional>
#include <variant>
#include <string_view>
#include <vector>

class RobTopStringContainer {
protected:
    std::string m_sOriginalString;

    std::unordered_map<int, std::variant<std::string_view, int, float, bool>> m_mContainer;
    std::unordered_map<int, std::variant<
        std::function<std::variant<std::string_view, int, float, bool>(std::string_view inputString, int inputID)>,
        std::function<std::variant<std::string_view, int, float, bool>(std::string_view inputString, int inputID, int custom_variable)>
    >> m_mFunctionContainer;
    std::unordered_map<int, int> m_mFuncCustomArgList;

    virtual std::string variantToString(std::variant<std::string_view, int, float, bool> var);
public:
    virtual std::variant<std::string_view, int, float, bool> getVariable(int id);

    virtual void setParserForVariable(int index, std::function<std::variant<std::string_view, int, float, bool>(std::string_view inputString, int inputID)> func);
    virtual void setParserForVariable(std::vector<int> indexes, std::function<std::variant<std::string_view, int, float, bool>(std::string_view inputString, int inputID)> func);
    virtual void setParserForVariable(int index, std::function<std::variant<std::string_view, int, float, bool>(std::string_view inputString, int inputID, int customArgumentValue)> func, int carg);
    virtual void setParserForVariable(std::vector<int> indexes, std::function<std::variant<std::string_view, int, float, bool>(std::string_view inputString, int inputID, int customArgumentValue)> func, int carg);

    virtual void parse();

    virtual bool variableExists(int id);

    RobTopStringContainer(std::string str);
    RobTopStringContainer();
};