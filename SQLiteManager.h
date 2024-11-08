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
#include <array>

#include <cstdint>

// client row.
using SQLiteRow = std::map<std::string, std::variant<std::string, int, bool, uint32_t, uint64_t>>;
// server row. this is what sqlite backend sends back
using SQLiteServerRow = std::map<std::string, std::string>;

#define SQLITE_CALLBACK_FUNC void(SQLiteManager *, std::vector<SQLiteServerRow>&, bool)

class SQLiteManager {
protected:
    // thread which processes all actions made inside this sqlite instance
    std::thread _processThread;
    // used for leaving from sqlite instance
    std::promise<void> _exitSignal;
    // used for leaving from sqlite instance
    std::future<void> _futureSignal;
public:
    // backend instance
    sqlite3*    _database = NULL;
    // path to the database file
    std::string _databasePath = "";
    // queue of requested actions for this sqlite database. they are processed synchronically
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
    // create sqlite instance with database path
    static SQLiteManager *create(std::string databasePath);

    ~SQLiteManager();

    // push a row into table
    void pushRow(SQLiteRow &row, std::string table);

    // wipes table's contents
    void wipeTable(std::string table);

    // count rows inside table
    int countTable(std::string table);

    // execute query (synchronious)
    std::vector<SQLiteServerRow> syncQuery(std::string query);

    // get rows from table without conditions
    std::vector<SQLiteServerRow> getTable(std::string table, std::string columnOrdering, int rowsPerPage, int page);
    // get rows from table without conditions
    std::vector<SQLiteServerRow> getTable(std::string table, std::string columnOrdering, int page);

    // get rows from table
    std::vector<SQLiteServerRow> getTableWithCondition(std::string table, std::string columnOrdering, int rowsPerPage, int page, SQLiteRow &condition, std::array<SQLiteRow, 2> between = {}, bool useBetween = false);
    // get rows from table
    std::vector<SQLiteServerRow> getTableWithCondition(std::string table, std::string columnOrdering, int page, SQLiteRow &condition, std::array<SQLiteRow, 2> between = {}, bool useBetween = false);

    // update row in table
    void updateRow(std::string table, SQLiteRow &newRow, SQLiteRow &condition);

    std::function<SQLITE_CALLBACK_FUNC> getPlaceholderCallback();

    static int sqlite_callback(void *data, int columns, char **array1, char **array2);
    static void processQueue(SQLiteManager *self, std::future<void> signal);

    std::vector<SQLiteServerRow> getRandomEntries(std::string table, int size);
};

class SQLiteCallbackData {
public:
    // sqlite instance
    SQLiteManager *manager = nullptr;

    // internal array 1
    char **_array1 = nullptr;
    // internal array 2
    char **_array2 = nullptr;

    // parsed data
    std::vector<SQLiteServerRow> _result_vec = {};
};
