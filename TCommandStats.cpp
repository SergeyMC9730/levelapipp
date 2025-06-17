/**
 *  LevelAPI - Geometry Dash level cacher with search functionality and more.
    Copyright (C) 2025  Sergei Baigerov

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

#include "TCommandStats.hpp"
#include "TelegramInstance.hpp"
#include "lapi_database.h"
#include "Translation.h"
#include "Level.h"
#include <fmt/format.h>

using namespace LevelAPI;

Frontend::TCommandStats::TCommandStats() : TCommand("lstat", "levelapi stats") {

}

void Frontend::TCommandStats::run(enum CommandType type, UserContext *ctx) {
    if (type == CommandType::CTCommandCall) {
        std::string nodes;
        std::vector<std::string> names = {};
        for (auto node : _instance->m_pDB->m_vNodes) {
            nodes += fmt::format("<code>{}</code>, ", node->m_sInternalName);
            names.push_back(node->m_sInternalName);
        }

        if (!nodes.empty()) {
            nodes.pop_back();
            nodes.pop_back();

            std::string msg = Translation::getByKey("lapi.tg.stat.ask-node",
                _instance->m_pDB->m_vNodes.size(),
                nodes
            );

            sendMessage(ctx, msg, names);
        } else {
            sendMessage(ctx, Translation::getByKey("lapi.tg.stat.no-nodes"));
        }
    } else if (type == CommandType::CTButton) {
        std::map<std::string, DatabaseController::Node*> names = {};
        for (auto node : _instance->m_pDB->m_vNodes) {
            std::string nn = node->m_sInternalName;
            std::transform(nn.begin(), nn.end(), nn.begin(), ::tolower);
            names[nn] = node;
        }

        std::string req_node = getPressedButton().second;
        std::transform(req_node.begin(), req_node.end(), req_node.begin(), ::tolower);

        if (!names.count(req_node)) {
            sendMessage(ctx, Translation::getByKey("lapi.tg.stat.node-not-found"));
            return;
        }

        auto node = names[req_node];
        auto srv = node->createServer();

        LevelAPI::Backend::SearchFilter filter;

        filter.m_nLevelsPerPage = 5;
        filter.m_nPage = 1;
        filter.m_eSort = LevelAPI::Backend::SearchSort::SSLatestDBApperead;

        auto levels = node->getLevels(filter);
        std::string last_levels;

        for (auto level : levels) {
            std::string like_string;
            std::string like_emoji = Translation::getByKey("lapi.tg.stat.like-emoji");
            std::string download_string;

            if (translation_language == "ru") {
                std::function<std::string(int, const std::string &, const std::string &, const std::string &)> helper_func = [](int value, const std::string &a, const std::string &b, const std::string &c) {
                    std::string value_str = std::to_string(value);
                    const char ln[2] = {value_str[value_str.size() - 1], 0};
                    int last_number = std::stoi(ln);

                    switch (last_number) {
                        case 0: return a;
                        case 1: return b;
                        case 2: case 3: case 4: return c;
                        case 5: case 6: case 7: case 8: case 9: return a;
                    }

                    return a;
                };

                std::vector<std::string> str_table = {};
                if (level->m_nLikes < 0) {
                    str_table = {
                        Translation::getByKey("lapi.tg.stat.dislikes"),
                        Translation::getByKey("lapi.tg.stat.dislike"),
                        Translation::getByKey("lapi.tg.stat.dislikesA"),
                    };
                    like_emoji = Translation::getByKey("lapi.tg.stat.dislike-emoji");
                } else {
                    str_table = {
                        Translation::getByKey("lapi.tg.stat.likes"),
                        Translation::getByKey("lapi.tg.stat.like"),
                        Translation::getByKey("lapi.tg.stat.likesA"),
                    };
                }

                like_string = helper_func(level->m_nLikes, str_table[0], str_table[1], str_table[2]);

                str_table = {
                    Translation::getByKey("lapi.tg.stat.downloads"),
                    Translation::getByKey("lapi.tg.stat.download"),
                    Translation::getByKey("lapi.tg.stat.downloadsA"),
                };
                download_string = helper_func(level->m_nDownloads, str_table[0], str_table[1], str_table[2]);
            } else if (translation_language == "en") {
                if (level->m_nLikes < 0) {
                    like_emoji = Translation::getByKey("lapi.tg.stat.dislike-emoji");
                    if (level->m_nLikes == -1) {
                        like_string = Translation::getByKey("lapi.tg.stat.dislike");
                    } else {
                        like_string = Translation::getByKey("lapi.tg.stat.dislikes");
                    }
                } else {
                    if (level->m_nLikes == 1) {
                        like_string = Translation::getByKey("lapi.tg.stat.like");
                    } else {
                        like_string = Translation::getByKey("lapi.tg.stat.likes");
                    }
                }

                if (level->m_nDownloads == 1) {
                    download_string = Translation::getByKey("lapi.tg.stat.download");
                } else {
                    download_string = Translation::getByKey("lapi.tg.stat.downloads");
                }
            }

            if (level->m_nLikes < 0) {
                like_emoji = Translation::getByKey("lapi.tg.stat.dislike-emoji");
            } else {
                like_emoji = Translation::getByKey("lapi.tg.stat.like-emoji");
            }

            last_levels += Translation::getByKey("lapi.tg.stat.level",
                level->m_sLevelName,
                level->m_nLevelID,
                level->m_sUsername,
                level->m_nDownloads,
                download_string,
                std::abs(level->m_nLikes),
                like_emoji,
                like_string
            ) + "\n";
        }

        if (last_levels.empty()) {
            last_levels = Translation::getByKey("lapi.tg.stat.no-levels");
        }

        std::string final_response = Translation::getByKey("lapi.tg.stat.info",
            node->m_sInternalName,
            std::max(node->m_nCachedLevels, node->m_nTableLevels),
            node->m_uQueue->m_vCommandQueue.size(),
            node->m_uQueue->m_vResolverQueuedLevels.size(),
            srv->getServerName(),
            last_levels
        );

        sendMessage(ctx, final_response);
        // requestContextDeletion();
    }
}
