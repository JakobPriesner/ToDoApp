#include "Manager.hpp"

using namespace Reminder::Core;
using namespace Reminder::Api::Parser;
using namespace Reminder::Repository;
using namespace Reminder::Core::Model;
using namespace std;

Manager::Manager(ParserIf &givenParser, RepositoryIf &givenRepository)
        : parser(givenParser), repository(givenRepository) {
}

std::string Manager::postList(std::string request) {
    int const dummyId = -1;
    std::optional<List> parsedListOptional = parser.convertListToModel(dummyId, request);
    if (!parsedListOptional.has_value()) {
        return parser.getEmptyResponseString();
    }

    List parsedList = parsedListOptional.value();

    std::optional<List> postedList = repository.postList(parsedList.getTitle());

    if (postedList) {
        return parser.convertToApiString(postedList.value());
    } else {
        return parser.getEmptyResponseString();
    }
}

std::string Manager::postItem(int listId, std::string request) {
    int const dummyId = -1;
    std::optional parsedItemOptional = parser.convertItemToModel(dummyId, request);
    if (!parsedItemOptional.has_value()) {
        return parser.getEmptyResponseString();
    }

    Item item = parsedItemOptional.value();
    std::optional<Item> postedItem = repository.postItem(listId, item.getText(), item.getDate(), item.isFlagged());
    if (postedItem) {
        return parser.convertToApiString(postedItem.value());
    } else {
        return parser.getEmptyResponseString();
    }
}

std::string Manager::putList(int listId, std::string request) {
    std::optional<List> parsedListOptional = parser.convertListToModel(listId, request);

    if (!parsedListOptional.has_value()) {
        return parser.getEmptyResponseString();
    }

    List list = parsedListOptional.value();
    std::optional<List> putList = repository.putList(listId, list.getTitle());

    if (putList) {
        return parser.convertToApiString(putList.value());
    } else {
        return parser.getEmptyResponseString();
    }
}

std::string Manager::putItem(int itemId, std::string request) {
    std::optional parsedItemOptional = parser.convertItemToModel(itemId, request);
    if (!parsedItemOptional.has_value()) {
        return parser.getEmptyResponseString();
    }

    Item item = parsedItemOptional.value();
    std::optional<Item> putItem = repository.putItem(itemId, item.getText(), item.getDate(), item.isFlagged());

    if (putItem) {
        return parser.convertToApiString(putItem.value());
    } else {
        return parser.getEmptyResponseString();
    }
}

std::string Manager::getList(int listId) {
    std::optional<List> list = repository.getList(listId);
    if (list) {
        return parser.convertToApiString(list.value());
    } else {
        return parser.getEmptyResponseString();
    }
}

std::string Manager::getLists() {
    std::vector<List> lists = repository.getLists();

    return parser.convertToApiString(lists);
}

std::string Manager::getListToday() {
    std::optional<List> list = repository.getListToday();
    if (list) {
        return parser.convertToApiString(list.value());
    } else {
        return parser.getEmptyResponseString();
    }
}

std::string Manager::getListFlagged() {
    std::optional<List> list = repository.getListFlagged();
    if (list) {
        return parser.convertToApiString(list.value());
    } else {
        return parser.getEmptyResponseString();
    }
}

std::string Manager::getApp() {
    App app = repository.getApp();

    return parser.convertToApiString(app);
}

void Manager::deleteList(int listId) {
    repository.deleteList(listId);
}

void Manager::deleteItem(int itemId) {
    repository.deleteItem(itemId);
}
