#include "Account19.h"
#include "Account10.h"

#include <stdexcept>

void Account19::addParsers() {
    Account10::addParsers();

    m_pContainer->setParserForVariable(
        16, 
        [&](std::string input, int id) {
            try {
                return std::stoi(input);
            } catch (std::invalid_argument &e) {
                return 0;
            }
        }
    );
}

void Account19::parseFromString(std::string server_response) {
    Account10::parseFromString(server_response);

    GET_KEY(m_pContainer, 16, this->accountID, GKINT);
}