#pragma once
#include <vector>
#include <optional>
#include <string>
#include "model/Item.h"

class ItemRepository {
public:
    ItemRepository();

    std::vector<Item> findAll() const;
    std::optional<Item> findById(const std::string& id) const;
    Item save(Item item);
    bool update(const Item& item);
    bool remove(const std::string& id);

private:
    std::string dataPath_;

    std::vector<Item> load() const;
    void persist(const std::vector<Item>& items) const;
    std::string nextId(const std::vector<Item>& items) const;
};
