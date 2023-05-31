#include "Account10.h"
#include "Account19.h"
#include <stdexcept>

Account10::Account10() {
    m_pContainer = new RobTopStringContainer();
}
Account10::~Account10() {
    delete m_pContainer;
}

void Account10::addParsers() {
    m_pContainer->setParserForVariable(
        1, 
        [&](std::string input, int id) {
            return input;
        }
    );

    m_pContainer->setParserForVariable(
        2, 
        [&](std::string input, int id) {
            try {
                return std::stoi(input);
            } catch (std::invalid_argument &e) {
                return 0;
            }
        }
    );
}

void Account10::parseFromString(std::string server_response) {
    m_pContainer->setString(server_response);

    addParsers();

    m_pContainer->parse();

    GET_KEY(m_pContainer, 1, this->username, GKSTRING);
    GET_KEY(m_pContainer, 2, this->userID, GKINT);

    if (this->username.find("GDUsr") != std::string::npos) this->autoRenamed = true;
}