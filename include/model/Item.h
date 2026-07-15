#pragma once
#include <string>
#include "third_party/json.hpp"

struct Item {
    std::string id;
    std::string name;
    std::string description;
    int quantity;
    std::string createdAt;

    nlohmann::json toJson() const;
    static Item fromJson(const nlohmann::json& j);
};
