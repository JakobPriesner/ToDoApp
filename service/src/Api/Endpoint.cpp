#include "Endpoint.hpp"


using namespace Reminder::Api;
using namespace Reminder::Core;
using namespace crow;

Endpoint::Endpoint(SimpleApp &givenApp, Manager &givenManager) : app(givenApp),
                                                                 manager(givenManager) {
    registerRoutes();
}

Endpoint::~Endpoint() = default;

void Endpoint::registerRoutes() {
    CROW_ROUTE(app, "/api/app")
            ([this](const request &req, response &res) {
                std::string jsonApp = manager.getApp();
                res.write(jsonApp);
                res.end();
            });

    CROW_ROUTE(app, "/api/app/lists/today")
            .methods("GET"_method)([this](const request &req, response &res) {
                std::string jsonLists = "{}";

                switch (req.method) {
                    case HTTPMethod::Get: {
                        jsonLists = manager.getListToday();
                        break;
                    }
                    default: {
                        break;
                    }
                }

                res.write(jsonLists);
                res.end();
            });

    CROW_ROUTE(app, "/api/app/lists/flagged")
            .methods("GET"_method)([this](const request &req, response &res) {
                std::string jsonLists = "{}";

                switch (req.method) {
                    case HTTPMethod::Get: {
                        jsonLists = manager.getListFlagged();
                        break;
                    }
                    default: {
                        break;
                    }
                }

                res.write(jsonLists);
                res.end();
            });

    CROW_ROUTE(app, "/api/app/lists")
            .methods("GET"_method, "POST"_method)([this](const request &req, response &res) {
                std::string jsonLists = "{}";

                switch (req.method) {
                    case HTTPMethod::Get: {
                        jsonLists = manager.getLists(); // TODO: we probably don't need this
                        break;
                    }
                    case HTTPMethod::Post: {
                        jsonLists = manager.postList(req.body);
                        res.code = 201;
                        break;
                    }
                    default: {
                        break;
                    }
                }

                res.write(jsonLists);
                res.end();
            });

    CROW_ROUTE(app, "/api/app/lists/<int>")
            .methods("GET"_method, "PUT"_method, "DELETE"_method)(
                    [this](const request &req, response &res, int listId) {
                        std::string jsonList = "{}";

                        switch (req.method) {
                            case HTTPMethod::Get: {
                                jsonList = manager.getList(listId);
                                break;
                            }
                            case HTTPMethod::Put: {
                                jsonList = manager.putList(listId, req.body);
                                break;
                            }
                            case HTTPMethod::Delete: {
                                manager.deleteList(listId);
                                break;
                            }
                            default: {
                                break;
                            }
                        }

                        res.write(jsonList);
                        res.end();
                    });

    CROW_ROUTE(app, "/api/app/lists/<int>/items")
            .methods("POST"_method)([this](const request &req, response &res, int listId) {
                std::string jsonItem = "{}";

                switch (req.method) {
                    case HTTPMethod::Post: {
                        jsonItem = manager.postItem(listId, req.body);
                        res.code = 201;
                        break;
                    }
                    default: {
                        break;
                    }
                }

                res.write(jsonItem);
                res.end();
            });

    CROW_ROUTE(app, "/api/app/items/<int>")
            .methods("PUT"_method, "DELETE"_method)(
                    [this](const request &req, response &res, int itemId) {
                        std::string jsonItem = "{}";

                        switch (req.method) {
                            case HTTPMethod::Put: {
                                jsonItem = manager.putItem(itemId, req.body);
                                break;
                            }
                            case HTTPMethod::Delete: {
                                manager.deleteItem(itemId);
                                break;
                            }
                            default: {
                                break;
                            }
                        }

                        res.write(jsonItem);
                        res.end();
                    });
}
