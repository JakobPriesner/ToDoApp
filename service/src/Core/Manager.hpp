#pragma once

#include "Api/Parser/ParserIf.hpp"
#include "Repository/RepositoryIf.hpp"

namespace Reminder::Core {
    class Manager {
    private:
        Reminder::Repository::RepositoryIf &repository;
        Reminder::Api::Parser::ParserIf &parser;

    public:
        Manager(Reminder::Api::Parser::ParserIf &givenParser, Reminder::Repository::RepositoryIf &givenRepository);
        ~Manager() = default;

        std::string postList(std::string request);
        std::string postItem(int listId, std::string request);

        std::string putList(int listId, std::string request);
        std::string putItem(int itemId, std::string request);

        std::string getList(int listId);
        std::string getLists();
        std::string getListToday();
        std::string getListFlagged();

        std::string getApp();

        void deleteList(int listId);
        void deleteItem(int itemId);
    };
} // namespace Reminder::Core
