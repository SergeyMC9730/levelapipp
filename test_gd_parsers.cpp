#include "lapi_database.h"
#include "tests.h"

#include <iostream>
#include <thread>
#include <chrono>

#include "GDServer_BoomlingsLike21.h"

#include "Translation.h"

using namespace LevelAPI;
using namespace std::chrono_literals;
using namespace LevelAPI::Frontend;

void Tests::testGDParsers() {
    std::cout << Translation::getByKey("lapi.gdpastertests.start");

    std::thread tr(Tests::testGDParsersThread);
    tr.detach();

    return;
}
void Tests::testGDParsersThread() {
    auto gd = new Backend::GDServer_BoomlingsLike21("https://www.boomlings.com/database");
    auto levels = gd->getLevelsBySearchType(4, "", 0);
    
    std::cout << Translation::getByKey("lapi.gdpastertests.test1result", levels.size());

    std::this_thread::sleep_for(600ms);

    auto somelevel = gd->resolveLevelData(levels[0]);
    somelevel->save();
    
    levels.clear();
    delete gd;
    gd = nullptr;

    std::cout << Translation::getByKey("lapi.gdpastertests.complete");

    return;
}