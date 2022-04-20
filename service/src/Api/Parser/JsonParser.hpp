#pragma once

#include "ParserIf.hpp"
#include "rapidjson/document.h"

using namespace rapidjson;

namespace Reminder::Api::Parser {

    class JsonParser : public ParserIf {
    private:
        static constexpr auto EMPTY_JSON = "{}";

        static bool isValidList(Document const &document);
        static bool isValidItem(Document const &document);

        static std::string jsonValueToString(Value const &json);

        static Value getJsonValueFromModel(Core::Model::Item const &item, Document::AllocatorType &allocator);
        static Value getJsonValueFromModel(Core::Model::List const &list, Document::AllocatorType &allocator);
        static Value getJsonValueFromModel(Core::Model::App const &app, Document::AllocatorType &allocator);

    public:
        JsonParser() = default;
        ~JsonParser() override = default;

        std::string convertToApiString(Core::Model::List &list) override;
        std::string convertToApiString(Core::Model::Item &item) override;
        std::string convertToApiString(std::vector<Core::Model::List> &lists) override;
        std::string convertToApiString(Core::Model::App &app) override;

        std::optional<Core::Model::List> convertListToModel(int listId, std::string &request) override;
        std::optional<Core::Model::Item> convertItemToModel(int itemId, std::string &request) override;

        std::string getEmptyResponseString() override {
            return JsonParser::EMPTY_JSON;
        }
    };
} // namespace Reminder::Api::Parser
