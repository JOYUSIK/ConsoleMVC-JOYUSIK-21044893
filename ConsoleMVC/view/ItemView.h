#pragma once
#include <string>
#include <vector>
#include "model/Item.h"

class ItemView {
public:
    std::string showMenu() const;
    Item inputItem() const;
    std::string inputId(const std::string& action) const;
    Item inputUpdate(const Item& current) const;

    void showItemList(const std::vector<Item>& items) const;
    void showItemDetail(const Item& item) const;
    void showSuccess(const std::string& msg) const;
    void showError(const std::string& msg) const;
    bool confirm(const std::string& msg) const;
};
