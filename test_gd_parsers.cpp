#include "lapi_database.h"
#include "tests.h"

#include <iostream>
#include <thread>
#include <chrono>
#include "termcolor/include/termcolor/termcolor.hpp"

#include "GDServer_BoomlingsLike21.h"

using namespace LevelAPI;
using namespace std::chrono_literals;

void Tests::testGDParsers() {
    std::cout << termcolor::bright_cyan << "[LevelAPI] Running GD parser tests...\n" << termcolor::reset;

    std::thread tr(Tests::testGDParsersThread);
    tr.detach();

    return;
}
void Tests::testGDParsersThread() {
    auto gd = new Backend::GDServer_BoomlingsLike21(new std::string("https://www.boomlings.com/database"));
    auto levels = gd->getLevelsBySearchType(4);
    
    std::cout << termcolor::magenta << "[LevelAPI TEST 1] gd.cpp returned " << levels.size() << " levels\n" << termcolor::reset;

    int i = 0;
    while(i < levels.size()) {
        auto level = levels[i];
        // std::cout << "[LevelAPI TEST 1] Level " << i << " \"" << levels[i]->m_sLevelName->c_str() << "\" by " << levels[i]->m_sUsername->c_str() << " (" << levels[i]->m_sDescription->c_str() << ")" << std::endl;
        // delete levels[i];
        // levels[i] = nullptr;
        i++;
    }

    std::this_thread::sleep_for(600ms);

    auto somelevel = gd->resolveLevelData(levels[0]);
    // std::cout << termcolor::magenta << "[LevelAPI TEST 2] gd.cpp returned " << somelevel->m_sLevelString->c_str() << "\n" << termcolor::reset;
    somelevel->save();

    // i = 0;
    // while(i < levels.size()) {
    //     delete levels[i];
    //     levels[i] = nullptr;
    //     i++;
    // }
    levels.clear();
    delete gd;
    gd = nullptr;

    std::cout << termcolor::green << "[LevelAPI] GD parser test complete\n" << termcolor::reset;

    return;
}