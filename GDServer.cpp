/**
 *  LevelAPI - Geometry Dash level cacher with search functionality and more.
    Copyright (C) 2023  Sergei Baigerov

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <https://www.gnu.org/licenses/>.
*/

#include "GDServer.h"
#include "LevelRange.h"
#include "LevelRangeList.h"
#include "curl_backend.h"

#include "Translation.h"

#include "GDCrypto-patched/Include/GDCrypto/RobTopCipher.hpp"

using namespace gdcrypto;
using namespace LevelAPI::Backend;

std::vector<LevelAPI::LevelRange> _stub11 = {};

GDServer::GDServer() : _ranges(_stub11) {
    m_eStatus = GSS_ONLINE;
}
GDServer::GDServer(std::vector<LevelRange> list) : _ranges(list) {
    m_eStatus = GSS_ONLINE;
}
GDServer::GDServer(LevelRangeList list) : _ranges(list) {
    m_eStatus = GSS_ONLINE;
}
GDServer::~GDServer() {
    m_eStatus = GSS_OFFLINE;
}
LevelAPI::DatabaseController::Level *GDServer::getLevelMetaByID(int id, bool resolveAccountInfo, std::optional<CurlProxy> proxy) {
    return nullptr;
}

int GDServer::getMaxLevelPageSize() {
    return 0;
}
int GDServer::getMaxMapPackPageSize() {
    return 0;
}

std::vector<LevelAPI::DatabaseController::Level *> GDServer::getLevelsBySearchType(int type, std::string str, int page) {
    return {};
}
std::vector<LevelAPI::DatabaseController::Level *> GDServer::getLevelsBySearchType(int type) {
    return getLevelsBySearchType(type, "", 0);
}

LevelAPI::DatabaseController::Level *GDServer::resolveLevelData(LevelAPI::DatabaseController::Level *level, std::optional<CurlProxy> proxy) {
    return nullptr;
}

void GDServer::setDebug(bool d) {
	m_bDebug = d;
}
bool GDServer::getDebug() {
	return m_bDebug;
}

void GDServer::setCredentials(std::string u, std::string p) {
    m_sUsername = u;
    m_sPassword = p;
    RobTopCipher gjpCipher(gdcrypto::vecFromArray(keys::GJP_KEY));;
    m_sGJPPassword = gjpCipher.encode(m_sPassword);
}

GDServerUploadResult *GDServer::uploadLevel(DatabaseController::Level *level) {
    return nullptr;
}

bool GDServer::login() {
    return false;
}

int GDServer::getGameVersion() {
    return 0;
}

std::string GDServer::_getDownloadLevelEndpointName() {
    return "";
}
std::string GDServer::_getLevelListEndpointName() {
    return "";
}
std::string GDServer::_getLoginAccountEndpointName() {
    return "";
}
std::string GDServer::_getSecretValueStandard() {
    return "";
}
std::string GDServer::_getSecretValueExtra() {
    return "";
}

std::string GDServer::determineGVFromID(int id) {
    return _ranges[id];
}

std::string GDServer::getServerName() {
    return Frontend::Translation::getByKey("lapi.gdserver.name");
}

std::string GDServer::getServerIdentifier() {
    return "gdserver";
}