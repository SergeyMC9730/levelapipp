#include "GDLevelSaveable.h"
#include "GJGameLevel.h"
#include <string>

void GDLevelSaveable::setLevel(GJGameLevel *lvl) {
    this->m_pLevel = lvl;
}
void GDLevelSaveable::setFileName(std::string str) {
    this->m_sFileName = str;
}
void GDLevelSaveable::setDebug(bool debug) {
    this->m_bDebug = debug;
}

GJGameLevel *GDLevelSaveable::getLevel() {
    return m_pLevel;
}
std::string GDLevelSaveable::getFileName() {
    return m_sFileName;
}
bool GDLevelSaveable::getDebug() {
    return m_bDebug;
}

void GDLevelSaveable::generate() {
    return;
}
void GDLevelSaveable::parse() {
    return;
}