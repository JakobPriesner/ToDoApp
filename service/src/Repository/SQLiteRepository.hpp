#pragma once

#include "Repository/RepositoryIf.hpp"
#include "sqlite3.h"
#include <string>

namespace Reminder::Repository {

    class SQLiteRepository : public RepositoryIf {
    private:
        sqlite3 *database;

        void initialize();
        static void handleSQLError(int statementResult, char *errorMessage);

        static int getListCallback(void *data, [[maybe_unused]] int numberOfColumns, char **fieldValues,
                                   [[maybe_unused]] char **columnNames);
        static int getListFilteredCallback(void *data, [[maybe_unused]] int numberOfColumns, char **fieldValues,
                                           [[maybe_unused]] char **columnNames);

        static Core::Model::Item createItem(char **fieldValues, int startIndex);
        static Core::Model::List createList(char **fieldValues, int startIndex);
    public:
        SQLiteRepository();
        ~SQLiteRepository() override;

        std::optional<Core::Model::List> postList(std::string title) override;
        std::optional<Core::Model::Item> postItem(int listId, std::string text,
                                                  std::string date, bool flagged) override;

        std::optional<Core::Model::List> putList(int listId, std::string title) override;
        std::optional<Core::Model::Item> putItem(int itemId, std::string text,
                                                 std::string date, bool flagged) override;

        std::optional<Core::Model::List> getList(int listId) override;
        std::vector<Core::Model::List> getLists() override;
        std::optional<Core::Model::List> getListToday() override;
        std::optional<Core::Model::List> getListFlagged() override;

        Core::Model::App getApp() override;

        void deleteList(int listId) override;
        void deleteItem(int itemId) override;
    };
} // namespace Reminder::Repository
