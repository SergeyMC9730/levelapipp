#include "v1.level.search.h"

#include "../lapi_database.h"
#include "../Level.h"

#include "HTTPContentTypeJSON.h"
#include "HTTPContentTypeImage.h"
#include <stdexcept>

LevelAPI::v1::LevelSearchRequest::LevelSearchRequest() {
    this->request_name = "search levels with all sorts of filters";
    this->request_url = "/api/v1/level/search";
}

std::shared_ptr<http_response> LevelAPI::v1::LevelSearchRequest::render(const http_request &req) {
    std::string node = req.get_arg_flat("node").data();

    LevelAPI::Backend::SearchFilter filter;
    
    filter.m_sName = decodeURIComponent(req.get_arg_flat("name").data());
    filter.m_sDescription = decodeURIComponent(req.get_arg_flat("description").data());
    filter.m_sReleaseGV = req.get_arg_flat("actualGameVersion").data();
    filter.m_sUsername = decodeURIComponent(req.get_arg_flat("username").data());

    // printf("------ USERNAME: %s ------\n", filter.m_sUsername.c_str());
    // printf("------ DESCRIPT: %s ------\n", filter.m_sDescription.c_str());

    #define PARSE_VAL(key, type, value) try { data = req.get_arg_flat(key).data(); if (!data.empty()) value = (type)std::stoi(data); } catch (std::invalid_argument invarg) {}

    std::string data = "";

    filter.timestamp_start = 0;
    filter.timestamp_end = 0;

    bool graph = false;
    int graphMembers = 0;

    PARSE_VAL("songOfficial", bool, filter.m_bSongOfficial);
    PARSE_VAL("accountID", int, filter.m_nAID);
    PARSE_VAL("stars", int, filter.m_nStars);
    PARSE_VAL("page", int, filter.m_nPage);
    PARSE_VAL("userID", int, filter.m_nUID);
    PARSE_VAL("gameVersion", int, filter.m_nServerGV);
    PARSE_VAL("songID", int, filter.m_nStars);
    PARSE_VAL("timestampStart", uint64_t, filter.timestamp_start);
    PARSE_VAL("timestampEnd", uint64_t, filter.timestamp_end);
    PARSE_VAL("asGraph", int, graph);
    PARSE_VAL("graphMembers", int, graphMembers);
    PARSE_VAL("difficulty", int, filter.m_nDifficulty);

    uint64_t tA = filter.timestamp_start;
    uint64_t tB = filter.timestamp_end;

    if (tA == 0) {
        tA = time(NULL);
        filter.timestamp_start = tA;
    }

    if (tB == 0) {
        tA = 0;
        filter.timestamp_end = 0;
        filter.timestamp_start = 0;
    }

    // printf("t0: %ld; t1: %ld\n", filter.timestamp_start, filter.timestamp_end);

    std::string sort_type = req.get_arg_flat("sort").data();

    if (!sort_type.empty()) {
        filter.m_eSort = LevelAPI::Backend::SearchSort::SSNone;

        std::map<std::string, LevelAPI::Backend::SearchSort> sortmap = {
            {"most-liked", LevelAPI::Backend::SearchSort::SSMostLiked},
            {"most-downloaded", LevelAPI::Backend::SearchSort::SSMostDownloaded},
            {"last-appeared", LevelAPI::Backend::SearchSort::SSLatestDBApperead},
            {"recent", LevelAPI::Backend::SearchSort::SSRecentLevel},
            {"oldest", LevelAPI::Backend::SearchSort::SSOldestLevel},
            {"none", LevelAPI::Backend::SearchSort::SSNone}
        };

        if (sortmap.count(sort_type) > 0) {
            filter.m_eSort = sortmap[sort_type];
        }
    }

    auto node_object = DatabaseController::database->getNode(node);

    if (node_object == nullptr) {
        nlohmann::json response_fail;

        response_fail["response"] = -1;
        response_fail["node"] = node;

        return generateResponse(response_fail.dump(), HTTPContentTypeJSON(), 404);
    }

    if (graph) {
        std::vector<int> levels_arr;
	filter.m_nLevelsPerPage = 100;
        
        for (int i = 0; i < graphMembers; i++) {
            filter.timestamp_start = tA - (tB * graphMembers) + (tB * i);
            filter.timestamp_end = filter.timestamp_start + tB;

            auto levels = node_object->getLevels(filter);
            levels_arr.push_back((int)levels.size());

            for (auto lvl : levels) {
                delete lvl;
            }
        }

        // for (int i = 0; i < levels_arr.size(); i++) {
        //     printf("[%d]: %d\n", i, levels_arr.at(i));
        // }

        if (levels_arr.size() != 0) {
            std::filesystem::create_directory("graphs");

            srand(time(0));

            std::string filename = "graphs/" + std::to_string(rand()) + ".png";

            node_object->createGraph(levels_arr, filename);

            return sendFile(filename, HTTPContentTypeImage());
        }

        nlohmann::json resp;
        resp["response"] = -2;

        return generateResponse(resp.dump(), HTTPContentTypeJSON());
    }

    auto levels = node_object->getLevels(filter);

    nlohmann::json resp;

    resp["response"] = 0;
    resp["levels"] = nlohmann::json::array();

    int i = 0;
    while(i < levels.size()) {
        levels[i]->_jsonObject["hasLevelString"] = levels[i]->m_bHasLevelString;

        resp["levels"].push_back(levels[i]->_jsonObject);

        delete levels[i];
        levels[i] = nullptr;

        i++;
    }

    return generateResponse(resp.dump(), HTTPContentTypeJSON());
}
