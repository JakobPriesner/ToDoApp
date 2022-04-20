#pragma once

#include "List.hpp"
#include <vector>

namespace Reminder::Core::Model {
    // renamed from 'Reminder' to 'App'
    // due to namespace issues
    class App {
    public:
        App() = default;
        ~App() = default;

        [[nodiscard]] const std::vector<List> &getLists() const;
        [[nodiscard]] int getFlaggedCount() const;
        [[nodiscard]] int getTodayCount() const;

        void setLists(const std::vector<List> &givenLists);
        void setFlaggedCount(int flaggedCount);
        void setTodayCount(int todayCount);
    private:
        std::vector<List> lists;
        int flaggedCount{};
        int todayCount{};
    };
} // namespace Reminder::Core::Model
