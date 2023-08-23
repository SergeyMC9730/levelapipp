#include "v1.level.search.h"

#include "../lapi_database.h"

#include "HTTPContentTypeJSON.h"
#include <stdexcept>

LevelAPI::v1::LevelSearchRequest::LevelSearchRequest() {
    this->request_name = "search levels with all sorts of filters";
    this->request_url = "/api/v1/level/search";
}

std::shared_ptr<http_response> LevelAPI::v1::LevelSearchRequest::render(const http_request &req) {
    std::string node = req.get_arg_flat("node").data();

    auto filter = new LevelAPI::Backend::SearchFilter();
    
    filter->m_sName = req.get_arg_flat("name").data();
    filter->m_sDescription = req.get_arg_flat("description").data();
    filter->m_sReleaseGV = req.get_arg_flat("actualGameVersion").data();

    #define PARSE_VAL(key, type, value) try { data = req.get_arg_flat(key).data(); if (!data.empty()) value = (type)std::stoi(data); } catch (std::invalid_argument invarg) {}

    std::string data = "";

    PARSE_VAL("songOfficial", bool, filter->m_bSongOfficial);
    PARSE_VAL("accountID", int, filter->m_nAID);
    PARSE_VAL("stars", int, filter->m_nStars);
    PARSE_VAL("page", int, filter->m_nPage);
    PARSE_VAL("userID", int, filter->m_nUID);
    PARSE_VAL("gameVersion", int, filter->m_nServerGV);
    PARSE_VAL("songID", int, filter->m_nStars);

    std::string sort_type = req.get_arg_flat("sort").data();

    filter->m_eSort = LevelAPI::Backend::SearchSort::SSNone;

    std::map<std::string, LevelAPI::Backend::SearchSort> sortmap = {
        {"most-liked", LevelAPI::Backend::SearchSort::SSMostLiked},
        {"most-downloaded", LevelAPI::Backend::SearchSort::SSMostDownloaded},
        {"last-appeared", LevelAPI::Backend::SearchSort::SSLatestDBApperead},
        {"recent", LevelAPI::Backend::SearchSort::SSRecentLevel},
        {"oldest", LevelAPI::Backend::SearchSort::SSOldestLevel},
        {"none", LevelAPI::Backend::SearchSort::SSNone}
    };

    if (sortmap.count(sort_type) > 0) {
        filter->m_eSort = sortmap[sort_type];
    }

    auto node_object = DatabaseController::database->getNode(node);

    if (node_object == nullptr) {
        nlohmann::json response_fail;

        response_fail["response"] = -1;
        response_fail["node"] = node;

        delete filter;
        filter = nullptr;

        return generateResponse(response_fail.dump(), HTTPContentTypeJSON(), 404);
    }

    auto levels = node_object->getLevels(filter);

    delete filter;
    filter = nullptr;

    nlohmann::json resp;

    resp["response"] = 0;
    resp["levels"] = nlohmann::json::array();

    int i = 0;
    while(i < levels.size()) {
        resp["levels"].push_back(levels[i]->_jsonObject);

        delete levels[i];
        levels[i] = nullptr;

        i++;
    }

    return generateResponse(resp.dump(), HTTPContentTypeJSON());
}
