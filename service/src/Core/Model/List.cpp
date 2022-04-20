#include "List.hpp"

#include <utility>

using namespace Reminder::Core::Model;

List::List(int id, std::string title) : listId(id), title(std::move(title)) {}

int List::getId() const {
    return listId;
}

const std::string &List::getTitle() const {
    return title;
}

const std::vector<Item> &List::getItems() const {
    return items;
}

void List::setId(int givenId) {
    this->listId = givenId;
}

void List::setTitle(const std::string &givenTitle) {
    this->title = givenTitle;
}

void List::addItem(const Item &item) {
    this->items.push_back(item);
}

