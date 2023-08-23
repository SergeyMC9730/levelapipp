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

#include "SQLiteManager.h"
#include <chrono>
#include <exception>
#include <functional>
#include <thread>
#include <utility>
#include <variant>
#include <csignal>
#include <iostream>

bool SQLiteManager_init = false;

void SQLiteManager::earlyInit() {
    sqlite3_initialize();
    sqlite3_enable_shared_cache(1);
    sqlite3_config(SQLITE_CONFIG_MULTITHREAD, 1);
}

SQLiteManager::~SQLiteManager() {
    // waiting for all queries to be processed
    while(!_queue.empty()) {
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
    }

    _exitSignal.set_value();
    
    sqlite3_close(_database);
}

SQLiteManager *SQLiteManager::create(std::string databasePath) {
    auto manager = new SQLiteManager(databasePath);

    if (!manager) return nullptr;

    return manager;
}

SQLiteManager::SQLiteManager(std::string databasePath) {
    if (!SQLiteManager_init) {
        earlyInit();

        SQLiteManager_init = true;
    }

    _database = NULL;

    _databasePath = databasePath;

    sqlite3_open(_databasePath.c_str(), &_database);

    if (!_database) return;

    _futureSignal = _exitSignal.get_future();

    _processThread = std::thread(SQLiteManager::processQueue, this, std::move(_futureSignal));
    _processThread.detach();
}

void SQLiteManager::updateRow(std::string table, SQLiteRow newRow, SQLiteRow condition) {
    std::string s = "UPDATE " + table + " SET";

    for (auto const [key, val] : newRow) {
        s += " " + key + " = ";

        if (std::holds_alternative<std::string>(val)) {
            // prevent sql injection

            auto buf = (char *)sqlite3_malloc(1024);
            auto str = std::get<std::string>(val);

            sqlite3_snprintf(1024, buf, "%q", str.c_str());

            std::string new_str = buf;

            s += "'" + new_str + "'";

            sqlite3_free(buf);
        }
        if (std::holds_alternative<int>(val)) {
            s += std::to_string(std::get<int>(val));
        }
        if (std::holds_alternative<uint32_t>(val)) {
            s += std::to_string((int)std::get<uint32_t>(val));
        }
        if (std::holds_alternative<uint64_t>(val)) {
            s += std::to_string(std::get<uint64_t>(val));
        }
        if (std::holds_alternative<bool>(val)) {
            s += std::to_string((int)std::get<bool>(val));
        }

        s += ",";
    }

    s.erase(s.size() - 1);

    s += " WHERE ";

    for (auto const [key, val] : condition) {
        s += key + " ";

        if (std::holds_alternative<std::string>(val)) {
            // prevent sql injection

            auto buf = (char *)sqlite3_malloc(1024);
            auto str = std::get<std::string>(val);

            sqlite3_snprintf(1024, buf, "%q", str.c_str());

            std::string new_str = buf;

            s += "LIKE '%" + new_str + "%'";

            sqlite3_free(buf);
        }
        if (std::holds_alternative<int>(val)) {
            s += "= " + std::to_string(std::get<int>(val));
        }
        if (std::holds_alternative<uint32_t>(val)) {
            s += "= " + std::to_string((int)std::get<uint32_t>(val));
        }
        if (std::holds_alternative<uint64_t>(val)) {
            s += "= " + std::to_string(std::get<uint64_t>(val));
        }
        if (std::holds_alternative<bool>(val)) {
            s += "= " + std::to_string(std::get<bool>(val));
        }

        s += " AND ";
    }

    s.erase(s.size() - 5);

    _queue.push(
        std::pair<
            std::string, 
            std::function<SQLITE_CALLBACK_FUNC>>
                (s, getPlaceholderCallback())
    );
}

void SQLiteManager::wipeTable(std::string table) {
    _queue.push(
        std::pair<
            std::string, 
            std::function<SQLITE_CALLBACK_FUNC>>
                ("DELETE FROM " + table + " WHERE 1", getPlaceholderCallback())
    );
}

void SQLiteManager::pushRow(SQLiteRow row, std::string table) {
    std::string s = "INSERT INTO " + table + " (";

    for (auto const [key, val] : row) {
        s += key + ", ";
    }

    s.erase(s.size() - 2);

    s += ") VALUES (";

    for (auto const [key, val] : row) {
        if (std::holds_alternative<std::string>(val)) {
            // prevent sql injection

            auto buf = (char *)sqlite3_malloc(1024);
            auto str = std::get<std::string>(val);

            sqlite3_snprintf(1024, buf, "%q", str.c_str());

            std::string new_str = buf;

            s += "'" + new_str + "'";

            sqlite3_free(buf);
        }
        if (std::holds_alternative<int>(val)) {
            s += std::to_string(std::get<int>(val));
        }
        if (std::holds_alternative<uint32_t>(val)) {
            s += std::to_string((int)std::get<uint32_t>(val));
        }
        if (std::holds_alternative<uint64_t>(val)) {
            s += std::to_string(std::get<uint64_t>(val));
        }
        if (std::holds_alternative<bool>(val)) {
            s += std::to_string((int)std::get<bool>(val));
        }

        s += ", ";
    }

    s.erase(s.size() - 2);

    s += ")";

    // std::cout << s << std::endl;
    
    _queue.push(
        std::pair<
            std::string, 
            std::function<SQLITE_CALLBACK_FUNC>>
                (s, getPlaceholderCallback())
    );

    return;
}

int SQLiteManager::sqlite_callback(void *data, int columns, char **array1, char **array2) {
    SQLiteCallbackData *c = static_cast<SQLiteCallbackData *>(data);
    int i = 0;

    std::map<std::string, std::string> m = {};

    c->_array1 = array1;
    c->_array2 = array2;

    while (i < columns) {
        m.insert(
            std::pair<std::string, std::string>(
                array2[i], array2[i + columns]
            )
        );

        i++;
    }

    c->_result_vec.push_back(m);

    // sqlite3_free_table(array1);
    // sqlite3_free_table(array2);

    return 0;
}

void SQLiteManager::processQueue(SQLiteManager *self, std::future<void> signal) {
    while (signal.wait_for(std::chrono::milliseconds(1)) == std::future_status::timeout) {
        bool had_request = false;
        if (self->_queue.size() != 0) {
            had_request = true;

            auto pair = self->_queue.front();

            char *error = NULL;
            auto callbackData = new SQLiteCallbackData();
            callbackData->manager = self;

            // std::cout << pair.first << std::endl;

            sqlite3_exec(self->_database, pair.first.c_str(), SQLiteManager::sqlite_callback, (void *)callbackData, &error);
        
            pair.second(self, callbackData->_result_vec, error != NULL);

            if (error) {
                std::cout << error << std::endl;

                sqlite3_free(error);
                error = NULL;
            }

            // sqlite3_free_table(callbackData->_array1);
            // sqlite3_free_table(callbackData->_array2);

            delete callbackData;
            callbackData = nullptr;

            self->_queue.pop();
        }
        if (!had_request) std::this_thread::sleep_for(std::chrono::milliseconds(10));
    }
}

std::vector<SQLiteServerRow> SQLiteManager::getTable(std::string table, std::string columnOrdering, int rowsPerPage, int page) {
    if (page == 0) page = 1;
    if (rowsPerPage == 0) rowsPerPage = 1;

    char *data1 = sqlite3_mprintf("SELECT * FROM %s ORDER BY %s LIMIT %d OFFSET %d;", table.c_str(), columnOrdering.c_str(), rowsPerPage, (page - 1) * rowsPerPage);

    auto vec = syncQuery(data1);

    sqlite3_free(data1);
    
    return vec;
}

std::vector<std::map<std::string, std::string>> SQLiteManager::getTableWithCondition(std::string table, std::string columnOrdering, int rowsPerPage, int page, SQLiteRow condition) {
    if (page == 0) page = 1;
    if (rowsPerPage == 0) rowsPerPage = 1;

    std::string eq = "";

    if (condition.size() != 0) {
        for (auto const [key, val] : condition) {

            eq += key + " ";

            if (std::holds_alternative<std::string>(val)) {
                // prevent sql injection

                auto buf = (char *)sqlite3_malloc(1024);
                auto str = std::get<std::string>(val);

                sqlite3_snprintf(1024, buf, "%q", str.c_str());

                std::string new_str = buf;

                eq += "LIKE '%" + new_str + "%'";

                sqlite3_free(buf);
            }
            if (std::holds_alternative<int>(val)) {
                eq += "= " + std::to_string(std::get<int>(val));
            }
            if (std::holds_alternative<uint32_t>(val)) {
                eq += "= " + std::to_string((int)std::get<uint32_t>(val));
            }
            if (std::holds_alternative<uint64_t>(val)) {
                eq += "= " + std::to_string(std::get<uint64_t>(val));
            }
            if (std::holds_alternative<bool>(val)) {
                eq += "= " + std::to_string(std::get<bool>(val));
            }

            eq += " AND ";
        }
    }

    if (eq.size() >= 5) eq.erase(eq.size() - 5);

    char *data1 = nullptr;

    if (condition.size() != 0) {
        data1 = sqlite3_mprintf("SELECT * FROM %s WHERE %s ORDER BY %s LIMIT %d OFFSET %d;", table.c_str(), eq.c_str(), columnOrdering.c_str(), rowsPerPage, (page - 1) * rowsPerPage);
    } else {
        data1 = sqlite3_mprintf("SELECT * FROM %s ORDER BY %s LIMIT %d OFFSET %d", table.c_str(), columnOrdering.c_str(), rowsPerPage, (page - 1) * rowsPerPage);
    }

    auto vec = syncQuery(data1);

    sqlite3_free(data1);
    
    return vec;
}


std::vector<SQLiteServerRow> SQLiteManager::getTableWithCondition(std::string table, std::string columnOrdering, int page, SQLiteRow condition) {
    return getTableWithCondition(table, columnOrdering, 10, page, condition);
}

std::vector<SQLiteServerRow> SQLiteManager::getTable(std::string table, std::string columnOrdering, int page) {
    return getTable(table, columnOrdering, 10, page);
}

std::function<SQLITE_CALLBACK_FUNC> SQLiteManager::getPlaceholderCallback() {
    return [](SQLiteManager *, std::vector<std::map<std::string, std::string>>, bool){};
}

std::vector<SQLiteServerRow> SQLiteManager::syncQuery(std::string query) {
    bool job_completed = false;

    std::vector<SQLiteServerRow> vec = {};

    _queue.push(
        std::pair<
            std::string, 
            std::function<SQLITE_CALLBACK_FUNC>>
                (query.c_str(),
                    [&](SQLiteManager *self, std::vector<std::map<std::string, std::string>> v, bool c) {
                        vec = v;
                        job_completed = true;
                    }
                )
    );
    
    while (!job_completed) {
        std::this_thread::sleep_for(std::chrono::milliseconds(10));
    }

    return vec;
}

int SQLiteManager::countTable(std::string table) {
    auto vec = syncQuery("SELECT COUNT(*) AS length FROM " + table);

    return std::stoi(vec.at(0)["length"]);
}
