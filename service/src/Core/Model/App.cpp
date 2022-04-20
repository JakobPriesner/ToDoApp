#include "App.hpp"

using namespace Reminder::Core::Model;

const std::vector<List> &App::getLists() const {
    return lists;
}

void App::setLists(const std::vector<List> &givenLists) {
    this->lists = givenLists;
}

int App::getFlaggedCount() const {
    return flaggedCount;
}

void App::setFlaggedCount(int givenFlaggedCount) {
    this->flaggedCount = givenFlaggedCount;
}

int App::getTodayCount() const {
    return todayCount;
}

void App::setTodayCount(int givenTodayCount) {
    this->todayCount = givenTodayCount;
}
