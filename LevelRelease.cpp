#include "lapi_database.h"
#include "Tools.h"

using namespace LevelAPI::DatabaseController;

LevelRelease::~LevelRelease() {
    delete m_fActualVersion;
    m_fActualVersion = nullptr;
}