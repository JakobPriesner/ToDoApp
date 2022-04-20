#include "Item.hpp"

#include <utility>

using namespace Reminder::Core::Model;

Item::Item(int itemId, std::string text, std::string date, bool flagged)
        : itemId(itemId), text(std::move(text)), date(std::move(date)), flagged(flagged) {}

int Item::getId() const {
    return itemId;
}

const std::string &Item::getText() const {
    return text;
}

const std::string &Item::getDate() const {
    return date;
}

bool Item::isFlagged() const {
    return flagged;
}

void Item::setId(int givenId) {
    this->itemId = givenId;
}

void Item::setText(const std::string &givenText) {
    this->text = givenText;
}

void Item::setDate(const std::string &givenDate) {
    this->date = givenDate;
}

void Item::setFlagged(bool givenFlagged) {
    this->flagged = givenFlagged;
}
