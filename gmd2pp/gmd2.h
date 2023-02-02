#pragma once

#include "level-converter/GJGameLevel.h"
#include "gmdtools.h"

class GMD2 {
protected:
    GJGameLevel *m_pLevel;
    GMDTools *m_pTools;

    std::string m_sFileName;
    int m_nSongName;
    bool m_bDebug;
public:
    GJGameLevel *getLevel();
    
    std::string getFileName();
    void setFileName(std::string str);

    int getSongID();
    void setSongID(int sid);

    bool getDebug();
    void setDebug(bool debug);

    virtual void parse();
    virtual void generate();

    GMD2();
    ~GMD2();
};