#pragma once

#include <string>

namespace Reminder::Core::Model {
    class Item {
    public:
        Item() = default;
        Item(int itemId, std::string text, std::string date, bool flagged);
        ~Item() = default;

        [[nodiscard]] int getId() const;
        [[nodiscard]] const std::string &getText() const;
        [[nodiscard]] const std::string &getDate() const;
        [[nodiscard]] bool isFlagged() const;

        void setId(int givenId);
        void setText(const std::string &givenText);
        void setDate(const std::string &givenDate);
        void setFlagged(bool givenFlagged);

    private:
        int itemId{};
        std::string text;
        std::string date;
        bool flagged{};
    };
} // namespace Reminder::Core::Model