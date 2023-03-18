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
    Backend::GDServer_BoomlingsLike21 gd = Backend::GDServer_BoomlingsLike21(new std::string(""));
    auto levels =  gd.getLevelsBySearch("4");
    
    std::cout << termcolor::magenta << "[LevelAPI TEST 1] gd.cpp returned " << levels.size() << " levels\n" << termcolor::reset;

    int i = 0;
    while(i < levels.size()) {
        std::cout << "[LevelAPI TEST 1] Level " << i << " \"" << *levels[i]->m_sLevelName << "\" by " << *levels[i]->m_sUsername << std::endl;
        i++;
    }

    return;
}