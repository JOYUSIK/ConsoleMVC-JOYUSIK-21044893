#include "model/Item.h"

nlohmann::json Item::toJson() const {
    return {
        {"id",          id},
        {"name",        name},
        {"description", description},
        {"quantity",    quantity},
        {"createdAt",   createdAt}
    };
}

Item Item::fromJson(const nlohmann::json& j) {
    Item item;
    item.id          = j.at("id").get<std::string>();
    item.name        = j.at("name").get<std::string>();
    item.description = j.at("description").get<std::string>();
    item.quantity    = j.at("quantity").get<int>();
    item.createdAt   = j.at("createdAt").get<std::string>();
    return item;
}
