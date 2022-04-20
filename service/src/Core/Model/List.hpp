#pragma once

#include <string>
#include <vector>
#include "Item.hpp"

namespace Reminder::Core::Model {
    class List {
    public:
        List() = default;
        List(int id, std::string name);
        ~List() = default;

        [[nodiscard]] int getId() const;
        [[nodiscard]] const std::string &getTitle() const;
        [[nodiscard]] const std::vector<Item> &getItems() const;

        void setId(int givenId);
        void setTitle(const std::string &givenTitle);
        void addItem(const Item &item);

    private:
        int listId{};
        std::string title;
        std::vector<Item> items;
    };
} // namespace Reminder::Core::Model
