#pragma once

#include "GJGameLevel.h"
#include "gmdtools.h"
#include "GDLevelSaveable.h"

class GMD2 : public GDLevelSaveable {
protected:
    GMDTools *m_pTools;
    int m_nSongName;
public:
    int getSongID();
    void setSongID(int sid);

    void parse() override;
    void generate() override;

    GMD2();
    ~GMD2();
};