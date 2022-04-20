#pragma once

#include "optional"
#include "Core/Model/List.hpp"
#include "Core/Model/App.hpp"

namespace Reminder::Repository {
    class RepositoryIf {
    public:
        virtual ~RepositoryIf() = default;

        virtual std::optional<Reminder::Core::Model::List> postList(std::string title) = 0;
        virtual std::optional<Reminder::Core::Model::Item> postItem(int listId, std::string text,
                                                                    std::string date, bool flagged) = 0;

        virtual std::optional<Reminder::Core::Model::List> putList(int listId, std::string title) = 0;
        virtual std::optional<Reminder::Core::Model::Item> putItem(int itemId, std::string text,
                                                                   std::string date, bool flagged) = 0;

        virtual std::optional<Reminder::Core::Model::List> getList(int listId) = 0;
        virtual std::vector<Reminder::Core::Model::List> getLists() = 0;
        virtual std::optional<Reminder::Core::Model::List> getListToday() = 0;
        virtual std::optional<Reminder::Core::Model::List> getListFlagged() = 0;

        virtual Reminder::Core::Model::App getApp() = 0;

        virtual void deleteList(int listId) = 0;
        virtual void deleteItem(int itemId) = 0;
    };

} // namespace Reminder::Repository
