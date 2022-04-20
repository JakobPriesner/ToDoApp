#include "SQLiteRepository.hpp"
#include <filesystem>
#include <iostream>

using namespace Reminder::Repository;
using namespace Reminder::Core::Model;
using namespace std;

#ifdef RELEASE_SERVICE
constexpr auto databaseFile = "./data/reminder-app.db";
#else
constexpr auto databaseFile = "../data/reminder-app.db";
#endif

SQLiteRepository::SQLiteRepository() : database(nullptr) {
    string databaseDirectory = filesystem::path(databaseFile).parent_path().string();

    if (!filesystem::is_directory(databaseDirectory)) {
        filesystem::create_directory(databaseDirectory);
    }

    int result = sqlite3_open(databaseFile, &database);

    if (SQLITE_OK != result) {
        cout << "Cannot open database: " << sqlite3_errmsg(database) << endl;
    }

    initialize();
}

SQLiteRepository::~SQLiteRepository() {
    sqlite3_close(database);
}

void SQLiteRepository::initialize() {
    int result;
    char *errorMessage = nullptr;

    string sqlCreateTableList =
            "CREATE TABLE IF NOT EXISTS list("
            "id INTEGER NOT NULL PRIMARY KEY AUTOINCREMENT,"
            "title TEXT NOT NULL);";

    string sqlCreateTableItem =
            "CREATE TABLE IF NOT EXISTS item("
            "id INTEGER NOT NULL PRIMARY KEY AUTOINCREMENT,"
            "text TEXT NOT NULL,"
            "date TEXT NOT NULL,"
            "flagged INTEGER NOT NULL,"
            "list_id INTEGER NOT NULL,"
            "FOREIGN KEY (list_id) REFERENCES list (id));";

    result = sqlite3_exec(database, sqlCreateTableList.c_str(), nullptr, nullptr, &errorMessage);
    handleSQLError(result, errorMessage);

    result = sqlite3_exec(database, sqlCreateTableItem.c_str(), nullptr, nullptr, &errorMessage);
    handleSQLError(result, errorMessage);
}

void SQLiteRepository::handleSQLError(int statementResult, char *errorMessage) {
    if (statementResult != SQLITE_OK) {
        cout << "SQL error: " << errorMessage << endl;
        sqlite3_free(errorMessage);
    }
}

int SQLiteRepository::getListCallback(void *data, [[maybe_unused]] int numberOfColumns, char **fieldValues,
                                      [[maybe_unused]] char **columnNames) {
    // Data looks like:
    // list.id  list.title   item.id          item.text     item.date   item.flagged
    //       2       test2         5           do stuff    1990-01-01              1
    //       2       test2         8     do other stuff    2021-04-05              0
    // ..............................................................................

    if (data && fieldValues) {
        auto *pLists = static_cast<std::vector<List> *>(data);
        std::vector<List> &lists = *pLists;

        Item item = createItem(fieldValues, 2);

        int columnId = fieldValues[0] ? stoi(fieldValues[0]) : -1;
        bool wasListAlreadyAdded = false;

        for (auto &list: lists) {
            if (list.getId() == columnId) {
                wasListAlreadyAdded = true;
                if (item.getId() != -1) {
                    list.addItem(item);
                }
                break;
            }
        }

        if (!wasListAlreadyAdded) {
            List list = createList(fieldValues, 0);

            if (item.getId() != -1) {
                list.addItem(item);
            }

            if (list.getId() != -1) {
                lists.push_back(list);
            }
        }
    }

    return 0;
}

int SQLiteRepository::getListFilteredCallback(void *data, [[maybe_unused]] int numberOfColumns, char **fieldValues,
                                              [[maybe_unused]] char **columnNames) {
    // Data looks like:
    // item.id          item.text     item.date   item.flagged
    //       5           do stuff    1990-01-01              1
    //       8     do other stuff    1990-01-01              0
    // ..............................................................................

    if (data && fieldValues) {
        auto *const list = static_cast<List *>(data);

        Item item = createItem(fieldValues, 0);

        list->addItem(item);
    }

    return 0;
}

Item SQLiteRepository::createItem(char **fieldValues, int startIndex) {
    Item item;

    item.setId(fieldValues[startIndex] ? stoi(fieldValues[startIndex]) : -1);
    startIndex++;

    item.setText(fieldValues[startIndex] ? fieldValues[startIndex] : "");
    startIndex++;

    item.setDate(fieldValues[startIndex] ? fieldValues[startIndex] : "");
    startIndex++;

    item.setFlagged(fieldValues[startIndex] != nullptr && stoi(fieldValues[startIndex]));

    return item;
}

List SQLiteRepository::createList(char **fieldValues, int startIndex) {
    List list;

    list.setId(fieldValues[startIndex] ? stoi(fieldValues[startIndex]) : -1);
    startIndex++;

    list.setTitle(fieldValues[startIndex] ? fieldValues[startIndex] : "");

    return list;
}

optional<List> SQLiteRepository::postList(string title) {
    string sqlPostList = "INSERT INTO list ('title') VALUES ('" + title + "');";

    int result;
    char *errorMessage = nullptr;

    result = sqlite3_exec(database, sqlPostList.c_str(), nullptr, nullptr, &errorMessage);
    handleSQLError(result, errorMessage);

    if (SQLITE_OK == result) {
        auto listId = static_cast<int>(sqlite3_last_insert_rowid(database));
        return Core::Model::List(listId, title);
    }

    return nullopt;
}

optional<Item> SQLiteRepository::postItem(int listId, string text, string date, bool flagged) {
    string sqlPostItem = "INSERT INTO item ('text', 'date', 'flagged', 'list_id') VALUES ('" +
                         text + "', '" + date + "', " + to_string(flagged) + ", " + to_string(listId) + ");";

    int result;
    char *errorMessage = nullptr;

    result = sqlite3_exec(database, sqlPostItem.c_str(), nullptr, nullptr, &errorMessage);
    handleSQLError(result, errorMessage);

    if (SQLITE_OK == result) {
        auto itemId = static_cast<int>(sqlite3_last_insert_rowid(database));

        return Core::Model::Item(itemId, text, date, flagged);
    }

    return {};
}

optional<List> SQLiteRepository::putList(int listId, std::string title) {
    string sqlPutList = "UPDATE list SET title = '" + title + "' WHERE id = " + to_string(listId) + ";";

    int result;
    char *errorMessage = nullptr;

    result = sqlite3_exec(database, sqlPutList.c_str(), nullptr, nullptr, &errorMessage);
    handleSQLError(result, errorMessage);

    // the kanban board adds the current items to the changed list, which the front-end doesn't actually need
    if (result == SQLITE_OK && sqlite3_changes(database) == 1)
        return List(listId, title);

    return {};
}

optional<Item> SQLiteRepository::putItem(int itemId, std::string text, std::string date, bool flagged) {
    string sqlPutItem =
            "UPDATE item SET text = '" + text + "', " + "date = '" + date + "', flagged = " + to_string(flagged) +
            " WHERE id = " + to_string(itemId) + ";";

    int result;
    char *errorMessage = nullptr;

    result = sqlite3_exec(database, sqlPutItem.c_str(), nullptr, nullptr, &errorMessage);
    handleSQLError(result, errorMessage);

    if (result == SQLITE_OK && sqlite3_changes(database) == 1)
        return Item(itemId, text, date, flagged);

    return {};
}

optional<List> SQLiteRepository::getList(int listId) {
    int result;
    char *errorMessage = nullptr;

    string sqlGetList = "SELECT list.id, list.title, item.id, item.text, item.date, item.flagged FROM list "
                        "LEFT JOIN item ON item.list_id = list.id WHERE list.id = " + std::to_string(listId) +
                        " ORDER BY item.date asc;";
    vector<List> lists;

    result = sqlite3_exec(database, sqlGetList.c_str(), getListCallback, &lists, &errorMessage);
    handleSQLError(result, errorMessage);

    if (result == SQLITE_OK && lists.size() == 1)
        return lists.front();

    return {};
}

vector<List> SQLiteRepository::getLists() {
    int result;
    char *errorMessage = nullptr;

    string sqlGetLists = "SELECT list.id, list.title, item.id, item.text, item.date, item.flagged FROM list "
                         "LEFT JOIN item ON item.list_id = list.id "
                         "ORDER BY list.id, item.date asc;";

    std::vector<List> lists;

    result = sqlite3_exec(database, sqlGetLists.c_str(), getListCallback, &lists, &errorMessage);
    handleSQLError(result, errorMessage);

    if (result == SQLITE_OK)
        return lists;

    return {};
}

optional<List> SQLiteRepository::getListToday() {
    int result;
    char *errorMessage = nullptr;

    string sqlGetListToday = "SELECT id, text, date, flagged FROM item "
                             "WHERE date = DATE('now');";
    List list;

    result = sqlite3_exec(database, sqlGetListToday.c_str(), getListFilteredCallback, &list, &errorMessage);
    handleSQLError(result, errorMessage);

    if (result == SQLITE_OK) {
        // let the front-end know it's filtered
        list.setId(-1);
        list.setTitle("Today");

        return list;
    }

    return {};
}

optional<List> SQLiteRepository::getListFlagged() {
    int result;
    char *errorMessage = nullptr;

    string sqlGetListToday = "SELECT id, text, date, flagged FROM item "
                             "WHERE flagged = 1;";
    List list;

    result = sqlite3_exec(database, sqlGetListToday.c_str(), getListFilteredCallback, &list, &errorMessage);
    handleSQLError(result, errorMessage);

    if (result == SQLITE_OK) {
        // let the front-end know it's filtered
        list.setId(-1);
        list.setTitle("Flagged");

        return list;
    }

    return {};
}

App SQLiteRepository::getApp() {
    App app;

    app.setLists(this->getLists());

    auto todayList = this->getListToday();
    auto flaggedList = this->getListFlagged();

    app.setTodayCount(todayList.has_value() ? static_cast<int>(todayList.value().getItems().size()) : 0);
    app.setFlaggedCount(flaggedList.has_value() ? static_cast<int>(flaggedList.value().getItems().size()) : 0);

    return app;
}

void SQLiteRepository::deleteList(int listId) {
    string sqlDeleteListItems = "DELETE FROM item WHERE list_id = " + to_string(listId) + ";";
    string sqlDeleteList = "DELETE FROM list WHERE id = " + to_string(listId) + ";";

    int result;
    char *errorMessage = nullptr;

    result = sqlite3_exec(database, sqlDeleteListItems.c_str(), nullptr, nullptr, &errorMessage);
    handleSQLError(result, errorMessage);

    result = sqlite3_exec(database, sqlDeleteList.c_str(), nullptr, nullptr, &errorMessage);
    handleSQLError(result, errorMessage);
}

void SQLiteRepository::deleteItem(int itemId) {
    string sqlDeleteItem = "DELETE FROM item WHERE item.id = " + to_string(itemId) + ";";

    int result;
    char *errorMessage = nullptr;

    result = sqlite3_exec(database, sqlDeleteItem.c_str(), nullptr, nullptr, &errorMessage);
    handleSQLError(result, errorMessage);
}
