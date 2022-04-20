#pragma once

#include <string>
#include "optional"
#include "Core/Model/App.hpp"

namespace Reminder::Api::Parser {
    class ParserIf {
    public:
        virtual ~ParserIf() = default;

        virtual std::string getEmptyResponseString() = 0;

        virtual std::string convertToApiString(Core::Model::List &list) = 0;
        virtual std::string convertToApiString(Core::Model::Item &item) = 0;
        virtual std::string convertToApiString(std::vector<Core::Model::List> &lists) = 0;
        virtual std::string convertToApiString(Core::Model::App &app) = 0;

        virtual std::optional<Core::Model::List> convertListToModel(int listId, std::string &request) = 0;
        virtual std::optional<Core::Model::Item> convertItemToModel(int itemId, std::string &request) = 0;
    };

} // namespace Reminder::Api::Parser