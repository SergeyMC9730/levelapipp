#include "gmd2.h"

#include <fstream>
#include <iterator>
#include <vector>

#include "zipfile_header.h"

#include "cJSON/cJSON.h"

#include <memory.h>

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

    std::istream* lmeta_stream = lmeta->GetDecompressionStream();
    std::istream* ldata_stream = ldata->GetDecompressionStream();

    std::vector<uint8_t> lmeta_buffer(std::istreambuf_iterator<char>(lmeta_stream[0]), {});
    std::vector<uint8_t> ldata_buffer(std::istreambuf_iterator<char>(ldata_stream[0]), {});

    cJSON *lmeta_object = cJSON_ParseWithLength((const char *)lmeta_buffer.data(), lmeta_buffer.size());
    char *leveldata = (char *)malloc(ldata_buffer.size());
    
    memcpy(leveldata, ldata_buffer.data(), ldata_buffer.size());

    

}