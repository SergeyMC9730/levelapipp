#include "Account21.h"
#include "Account19.h"

#include <stdexcept>

void Account21::addParsers() {
    Account19::addParsers();

    m_pContainer->setParserForVariable(
        49, 
        [&](std::string input, int id) {
            try {
                return std::stoi(input);
            } catch (std::invalid_argument &e) {
                return 0;
            }
        }
    );
}

void Account21::parseFromString(std::string server_response) {
    Account19::parseFromString(server_response);

    GET_KEY(m_pContainer, 49, this->modLevel, GKINT);
}