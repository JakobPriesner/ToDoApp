#include "Api/Endpoint.hpp"
#include "Api/Parser/JsonParser.hpp"
#include "Core/Manager.hpp"
#include "Repository/SQLiteRepository.hpp"
#include "crow.h"

int main() {
    crow::SimpleApp crowApplication;
    Reminder::Repository::SQLiteRepository sqliteRepository;
    Reminder::Api::Parser::JsonParser jsonParser;

    Reminder::Core::Manager manager(jsonParser, sqliteRepository);
    Reminder::Api::Endpoint endpoint(crowApplication, manager);

    crowApplication.port(8080)
            .multithreaded()
            .run();
}
