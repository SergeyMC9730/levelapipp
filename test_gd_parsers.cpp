#include "lapi_database.h"
#include "tests.h"

#include <iostream>
#include <thread>
#include "termcolor/include/termcolor/termcolor.hpp"

#include "GDServer_BoomlingsLike21.h"

using namespace LevelAPI;

void Tests::testGDParsers() {
    std::cout << termcolor::bright_cyan << "[LevelAPI] Running GD parser tests...\n" << termcolor::reset;

    std::thread tr(Tests::testGDParsersThread);
    tr.detach();

    return;
}
void Tests::testGDParsersThread() {
    auto gd = new Backend::GDServer_BoomlingsLike21(new std::string(""));
    auto levels = gd->getLevelsBySearch("4");
    
    std::cout << termcolor::magenta << "[LevelAPI TEST 1] gd.cpp returned " << levels.size() << " levels\n" << termcolor::reset;

    int i = 0;
    while(i < levels.size()) {
        auto level = levels[i];
        //std::cout << "[LevelAPI TEST 1] Level " << i << " \"" << levels[i]->m_sLevelName->c_str() << "\" by " << levels[i]->m_sUsername->c_str() << std::endl;
        delete levels[i];
        i++;
    }
    delete gd;
    levels.clear();

    std::cout << termcolor::green << "[LevelAPI] GD parser test complete\n" << termcolor::reset;

    return;
}