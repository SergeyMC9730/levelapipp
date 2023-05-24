#pragma once

#include "GJGameLevel.h"

#include <string>

class GDLevelSaveable {
protected:
    GJGameLevel *m_pLevel;
    std::string m_sFileName;
    bool m_bDebug;
public:
    virtual GJGameLevel *getLevel();
    virtual void setLevel(GJGameLevel *lvl);
    
    virtual std::string getFileName();
    virtual void setFileName(std::string str);

    virtual bool getDebug();
    virtual void setDebug(bool debug);

    virtual void parse();
    virtual void generate();
};