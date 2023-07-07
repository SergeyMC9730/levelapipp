#include "SQLiteManager.h"
#include <chrono>
#include <exception>
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

void SQLiteManager::wipeTable(std::string table) {
    _queue.push(
        std::pair<
            std::string, 
            std::function<SQLITE_CALLBACK_FUNC>>
                ("DELETE FROM " + table + " WHERE 1", getPlaceholderCallback())
    );
}

void SQLiteManager::pushRow(std::map<std::string, std::variant<std::string, int, bool>> row, std::string table) {
    std::string s = "INSERT INTO " + table + "(";

    for (auto const& [key, val] : row) {
        s += key + ", ";
    }

    s.pop_back();
    s.pop_back();

    s += ") VALUES (";

    for (auto const& [key, val] : row) {
        if (std::holds_alternative<std::string>(val)) {
            s += "'" + std::get<std::string>(val) + "'";
        }
        if (std::holds_alternative<int>(val)) {
            s += std::to_string(std::get<int>(val));
        }
        if (std::holds_alternative<bool>(val)) {
            s += std::to_string((int)std::get<bool>(val));
        }

        s += ", ";
    }

    s.pop_back();
    s.pop_back();

    s += ")";
    
    _queue.push(
        std::pair<
            std::string, 
            std::function<SQLITE_CALLBACK_FUNC>>
                (s, getPlaceholderCallback())
    );

    
}

int SQLiteManager::sqlite_callback(void *data, int columns, char **array1, char **array2) {
    SQLiteCallbackData *c = static_cast<SQLiteCallbackData *>(data);
    int i = 0;

    std::cout << "Columns: " << columns << std::endl;

    std::cout << "ARRAY 1:" << std::endl;
    while (array1[i] != NULL) {
        std::cout << array1[i] << std::endl; 
        i++;
    }

    i = 0;

    std::cout << "ARRAY 2:" << std::endl;
    while (array2[i] != NULL) {
        std::cout << array2[i] << std::endl; 
        i++;
    }

    return 0;
}

void SQLiteManager::processQueue(SQLiteManager *self, std::future<void> signal) {
    while (signal.wait_for(std::chrono::milliseconds(1)) == std::future_status::timeout) {
        if (self->_queue.size() != 0) {
            auto pair = self->_queue.front();

            char *error = NULL;
            auto callbackData = new SQLiteCallbackData();
            callbackData->manager = self;

            sqlite3_exec(self->_database, pair.first.c_str(), SQLiteManager::sqlite_callback, (void *)callbackData, &error);
        
            pair.second(self, callbackData->_result_map, error != NULL);

            if (error) {
                std::cout << error << std::endl;

                sqlite3_free(error);
                error = NULL;
            }

            delete callbackData;
            callbackData = nullptr;

            self->_queue.pop();
        }
        std::this_thread::sleep_for(std::chrono::milliseconds(10));
    }
}

std::function<SQLITE_CALLBACK_FUNC> SQLiteManager::getPlaceholderCallback() {
    return [](SQLiteManager *, std::map<std::string, std::string>, bool){};
}