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

#pragma  once

#include <sqlite3.h>
#include <string>
#include <queue>
#include <map>
#include <functional>
#include <thread>
#include <variant>
#include <future>
#include <vector>

#include <cstdint>

using SQLiteRow = std::map<std::string, std::variant<std::string, int, bool, uint32_t, uint64_t>>;
using SQLiteServerRow = std::map<std::string, std::string>;

#define SQLITE_CALLBACK_FUNC void(SQLiteManager *, std::vector<SQLiteServerRow>, bool)

class SQLiteManager {
protected:
    std::thread _processThread;
    std::promise<void> _exitSignal;
    std::future<void> _futureSignal;
public:
    sqlite3*    _database = NULL;
    std::string _databasePath = "";
    std::queue<
        std::pair<
            std::string, std::function<
                SQLITE_CALLBACK_FUNC
            >
        >
    > _queue = {};

private:
    SQLiteManager(std::string databasePath);

    void earlyInit();
public:
    static SQLiteManager *create(std::string databasePath);

    ~SQLiteManager();

    void pushRow(SQLiteRow row, std::string table);

    // wipes table's contents
    void wipeTable(std::string table);

    int countTable(std::string table);

    std::vector<SQLiteServerRow> syncQuery(std::string query);

    std::vector<SQLiteServerRow> getTable(std::string table, std::string columnOrdering, int rowsPerPage, int page);
    std::vector<SQLiteServerRow> getTable(std::string table, std::string columnOrdering, int page);

    std::vector<SQLiteServerRow> getTableWithCondition(std::string table, std::string columnOrdering, int rowsPerPage, int page, SQLiteRow condition);
    std::vector<SQLiteServerRow> getTableWithCondition(std::string table, std::string columnOrdering, int page, SQLiteRow condition);

    void updateRow(std::string table, SQLiteRow newRow, SQLiteRow condition);

    std::function<SQLITE_CALLBACK_FUNC> getPlaceholderCallback();

    static int sqlite_callback(void *data, int columns, char **array1, char **array2);
    static void processQueue(SQLiteManager *self, std::future<void> signal);
};

class SQLiteCallbackData {
public:
    SQLiteManager *manager = nullptr;

    char **_array1 = nullptr;
    char **_array2 = nullptr;

    std::vector<std::map<std::string, std::string>> _result_vec = {};
};