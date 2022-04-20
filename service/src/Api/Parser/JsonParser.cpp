#define RAPIDJSON_ASSERT(x)

#include "JsonParser.hpp"
#include "rapidjson/stringbuffer.h"
#include "rapidjson/writer.h"

using namespace Reminder::Api::Parser;
using namespace Reminder::Core::Model;
using namespace rapidjson;
using namespace std;


string JsonParser::jsonValueToString(Value const &json) {
    StringBuffer buffer;
    Writer<StringBuffer> writer(buffer);

    json.Accept(writer);

    return buffer.GetString();
}

Value JsonParser::getJsonValueFromModel(Reminder::Core::Model::List const &list,
                                        Document::AllocatorType &allocator) {
    Value jsonList(kObjectType);

    jsonList.AddMember("id", list.getId(), allocator);
    jsonList.AddMember("title", Value(list.getTitle().c_str(), allocator), allocator);

    Value jsonItems(kArrayType);

    for (Item const &item: list.getItems()) {
        Value jsonItem = getJsonValueFromModel(item, allocator);
        jsonItems.PushBack(jsonItem, allocator);
    }

    jsonList.AddMember("items", jsonItems, allocator);
    jsonList.AddMember("itemCounter", list.getItems().size(), allocator);

    return jsonList;
}

rapidjson::Value JsonParser::getJsonValueFromModel(Item const &item, rapidjson::Document::AllocatorType &allocator) {
    Value jsonItem(kObjectType);

    jsonItem.AddMember("id", item.getId(), allocator);
    jsonItem.AddMember("text", Value(item.getText().c_str(), allocator), allocator);
    jsonItem.AddMember("date", Value(item.getDate().c_str(), allocator), allocator);
    jsonItem.AddMember("flagged", item.isFlagged(), allocator);

    return jsonItem;
}

Value JsonParser::getJsonValueFromModel(const App &app, MemoryPoolAllocator<CrtAllocator> &allocator) {
    Value jsonApp(kObjectType);
    Value jsonLists(kArrayType);

    for (auto &list: app.getLists()) {
        Value jsonList = getJsonValueFromModel(list, allocator);
        jsonLists.PushBack(jsonList, allocator);
    }

    jsonApp.AddMember("lists", jsonLists, allocator);
    jsonApp.AddMember("flaggedCount", app.getFlaggedCount(), allocator);
    jsonApp.AddMember("todayCount", app.getTodayCount(), allocator);

    return jsonApp;
}

bool JsonParser::isValidList(Document const &document) {
    if (document.HasParseError() || !document["title"].IsString()) {
        return false;
    }

    return true;
}

bool JsonParser::isValidItem(const Document &document) {
    if (document.HasParseError() || !document["text"].IsString()
        || !document["date"].IsString() || !document["flagged"].IsBool()) {
        return false;
    }

    return true;
}

std::optional<List> JsonParser::convertListToModel(int listId, string &request) {
    Document document;
    document.Parse(request.c_str());

    if (isValidList(document)) {
        std::string title = document["title"].GetString();

        return List(listId, title);
    }

    return {};
}

std::string JsonParser::convertToApiString(List &list) {
    Document document(kObjectType);

    Value jsonList = getJsonValueFromModel(list, document.GetAllocator());

    return jsonValueToString(jsonList);
}

std::string JsonParser::convertToApiString(Item &item) {
    Document document(kObjectType);

    Value jsonItem = getJsonValueFromModel(item, document.GetAllocator());

    return jsonValueToString(jsonItem);
}

std::string JsonParser::convertToApiString(vector<List> &lists) {
    Document document(kArrayType);
    Document::AllocatorType &allocator = document.GetAllocator();

    for (auto &list: lists) {
        Value jsonList = getJsonValueFromModel(list, allocator);
        document.PushBack(jsonList, allocator);
    }

    return jsonValueToString(document);
}

std::string JsonParser::convertToApiString(App &app) {
    Document document;
    Document::AllocatorType &allocator = document.GetAllocator();

    Value jsonApp = getJsonValueFromModel(app, allocator);

    return jsonValueToString(jsonApp);
}

std::optional<Reminder::Core::Model::Item> JsonParser::convertItemToModel(int itemId, string &request) {
    Document document;
    document.Parse(request.c_str());

    if (isValidItem(document)) {
        auto text = document["text"].GetString();
        auto date = document["date"].GetString();
        auto flagged = document["flagged"].GetBool();

        return Item(itemId, text, date, flagged);
    }

    return {};
}
