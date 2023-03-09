#include "gmd2.h"
#include "hex.h"
#include "level-converter/base64.h"

#include <fstream>
#include <iterator>
#include <vector>
#include <sstream>

#include "zipfile_header.h"

#include "cJSON/cJSON.h"

#include <memory.h>

#include "pugixml_patch/src/pugixml.hpp"

GMD2::GMD2() {
    this->m_pLevel = new GJGameLevel();
    this->m_pTools = new GMDTools();
}

GMD2::~GMD2() {
    delete(this->m_pLevel);
    delete(this->m_pTools);
}

GJGameLevel *GMD2::getLevel() {
    return this->m_pLevel;
}

std::string GMD2::getFileName() {
    return this->m_sFileName;
}
void GMD2::setFileName(std::string str) {
    this->m_sFileName = str;
}

int GMD2::getSongID() {
    return this->m_nSongName;
}
void GMD2::setSongID(int sid) {
    this->m_nSongName = sid;
}

bool GMD2::getDebug() {
    return this->m_bDebug;
}
void GMD2::setDebug(bool debug) {
    this->m_bDebug = debug;
}

void GMD2::parse() {
    ZipArchive::Ptr data = ZipFile::Open(this->m_sFileName);

    ZipArchiveEntry::Ptr lmeta = data->GetEntry("level.meta");
    ZipArchiveEntry::Ptr ldata = data->GetEntry("level.data");

    if(!lmeta || !ldata) {
        if(this->m_bDebug) printf("[GMD2Impl] Did not found one of these files: level.meta, level.data. Is it GMD2 file?\n");

        return;
    }

    std::istream* lmeta_stream = lmeta->GetDecompressionStream();
    std::istream* ldata_stream = ldata->GetDecompressionStream();

    std::vector<uint8_t> lmeta_buffer(std::istreambuf_iterator<char>(lmeta_stream[0]), {});
    std::vector<uint8_t> ldata_buffer(std::istreambuf_iterator<char>(ldata_stream[0]), {});

    cJSON *lmeta_object = cJSON_ParseWithLength((const char *)lmeta_buffer.data(), lmeta_buffer.size());
    char *leveldata = (char *)malloc(ldata_buffer.size());
    
    memcpy(leveldata, ldata_buffer.data(), ldata_buffer.size());

    pugi::xml_document doc;
    pugi::xml_parse_result result = doc.load_buffer(leveldata, ldata_buffer.size());

    auto xml_levelname = doc.child("d").child("s").first_child().value();
    auto xml_leveldesc = doc.child("d").child("s").next_sibling().next_sibling().first_child().value();
    auto xml_leveldata = doc.child("d").child("s").next_sibling().next_sibling().next_sibling().next_sibling().first_child().value();
    auto xml_iscustoms = doc.child("d").child("s").next_sibling().next_sibling().next_sibling().next_sibling().next_sibling().first_child().value();
    auto xml_songid = doc.child("d").child("s").next_sibling().next_sibling().next_sibling().next_sibling().next_sibling().first_child().parent().next_sibling().first_child().value();

    this->m_pLevel->m_sLevelString = std::string(xml_leveldata);
    this->m_pLevel->m_sDescription = convertHexToStr(std::string(xml_leveldesc));
    this->m_pLevel->m_sLevelName = std::string(xml_levelname);
    if(!strcmp(xml_iscustoms, "k45")) {
        this->m_pLevel->m_nMusicID = std::stoi(std::string(xml_songid)) - 1;
    } else {
        this->m_pLevel->m_nSongID = std::stoi(std::string(xml_songid));
    }

    if(this->m_bDebug) {
        printf("[GMD2Impl] Parsed level \"%s\"\n", this->m_pLevel->m_sLevelName.data());
    }

    return;
}

void GMD2::generate() {
    std::string lmeta = "{\"compression\":\"none\"}";
    std::string ldata = "";

    ldata += "<d><k>kCEK</k>";
        if(this->m_bDebug) printf("[GMD2Impl] Level Name: %s\n", this->m_pLevel->m_sLevelName.data());
        ldata += "<i>4</i><k>k2</k>";
            ldata += "<s>";
            ldata += this->m_pLevel->m_sLevelName;
            ldata += "</s>";
        
        if(this->m_bDebug) printf("[GMD2Impl] Level Description: %s\n", this->m_pLevel->m_sDescription.data());
        ldata += "<k>k3</k>";
            ldata += "<s>";
                ldata += base64_encode((BYTE *)this->m_pLevel->m_sDescription.data(), this->m_pLevel->m_sDescription.size());
            ldata += "</s>";

        ldata += "<k>k4</k>";
            ldata += "<s>";
                ldata += this->m_pLevel->m_sLevelString;
            ldata += "</s>";

        std::string knum = (this->m_pLevel->m_nSongID == 0) ? "k45" : "k8";
        std::string song = "";
            song += "<k>";
                song += knum;
            song += "</k>";
            
            song += "<i>";
                char *buf = (char *)malloc(128);
                sprintf(buf, "%d", (this->m_pLevel->m_nSongID == 0) ? this->m_pLevel->m_nMusicID - 1: this->m_pLevel->m_nSongID);
                song += buf;
            song += "</i>";
        song += "<k>k13</k><t/><k>k21</k><i>2</i><k>k50</k><i>35</i>";

        ldata += song;
    ldata += "</d>";

    free(buf);

    // if(this->m_nSongName != -1) {
    //     this->m_pTools->setURL(this->m_pLevel->)
    // }

    ZipArchive::Ptr arc = ZipArchive::Create();
    ZipArchiveEntry::Ptr pLm = arc->CreateEntry("level.meta");
    ZipArchiveEntry::Ptr pLd = arc->CreateEntry("level.data");

    std::istringstream str1 = std::istringstream(lmeta);
    std::istringstream str2 = std::istringstream(ldata);

    pLm->SetCompressionStream(str1, StoreMethod::Create());
    pLd->SetCompressionStream(str2, StoreMethod::Create());

    ZipFile::SaveAndClose(arc, this->getFileName());
}

void GMD2::setLevel(GJGameLevel *lvl) {
    this->m_pLevel = lvl;
}