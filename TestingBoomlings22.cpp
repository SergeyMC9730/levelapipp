#include "TestingBoomlings22.h"

#include "GDServer_Boomlings22.h"

#include <iostream>
#include <thread>

using namespace LevelAPI;

void Tests::test_boomlings_ver22() {
    std::cout << "Testing official GD server in 2.2 mode" << std::endl;

    auto boom = new LevelAPI::Backend::GDServer_Boomlings22("https://www.boomlings.com/database");

    boom->setDebug(false);

    std::cout << "New feature: download levels in batch. (4 levels)" << std::endl;

    auto levels = boom->fetchListOfLevels({128, 91682434, 91698864, 91707733});

    int i = 0;
    while(i < levels.size()) {
        auto level = levels[i];

        std::cout << " - Level \"" << level->m_sLevelName << "\" (" << std::to_string(level->m_nLevelID) << ")" << std::endl;
        
        delete level;
        levels[i] = nullptr;
        level = nullptr;

        i++;
    }

    delete boom;

    boom = nullptr;

    std::cout << "2.2 test has been completed" << std::endl;
    
    return;
}