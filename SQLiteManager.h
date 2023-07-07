#pragma  once

#include <sqlite3.h>
#include <string>
#include <queue>
#include <map>
#include <functional>
#include <thread>
#include <variant>
#include <future>

#define SQLITE_CALLBACK_FUNC void(SQLiteManager *, std::map<std::string, std::string>, bool)

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

    void pushRow(std::map<std::string, std::variant<std::string, int, bool>> row, std::string table);

    // wipes table's contents
    void wipeTable(std::string table);

    std::function<SQLITE_CALLBACK_FUNC> getPlaceholderCallback();

    static int sqlite_callback(void *data, int columns, char **array1, char **array2);
    static void processQueue(SQLiteManager *self, std::future<void> signal);
};

class SQLiteCallbackData {
public:
    SQLiteManager *manager = nullptr;

    std::map<std::string, std::string> _result_map = {};
};