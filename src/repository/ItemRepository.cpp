#include "repository/ItemRepository.h"
#include <fstream>
#include <filesystem>
#include <stdexcept>

namespace fs = std::filesystem;

static const std::string DATA_PATH = "data/items.json";

ItemRepository::ItemRepository() : dataPath_(DATA_PATH) {
    fs::create_directories("data");
    if (!fs::exists(dataPath_)) {
        std::ofstream f(dataPath_);
        f << "[]";
    }
}

std::vector<Item> ItemRepository::findAll() const {
    return load();
}

std::optional<Item> ItemRepository::findById(const std::string& id) const {
    for (const auto& item : load()) {
        if (item.id == id) return item;
    }
    return std::nullopt;
}

Item ItemRepository::save(Item item) {
    auto items = load();
    item.id = nextId(items);
    items.push_back(item);
    persist(items);
    return item;
}

bool ItemRepository::update(const Item& item) {
    auto items = load();
    for (auto& stored : items) {
        if (stored.id == item.id) {
            stored = item;
            persist(items);
            return true;
        }
    }
    return false;
}

bool ItemRepository::remove(const std::string& id) {
    auto items = load();
    auto before = items.size();
    items.erase(std::remove_if(items.begin(), items.end(),
        [&id](const Item& i) { return i.id == id; }), items.end());
    if (items.size() == before) return false;
    persist(items);
    return true;
}

std::vector<Item> ItemRepository::load() const {
    std::ifstream f(dataPath_);
    if (!f.is_open()) throw std::runtime_error("데이터 파일을 열 수 없습니다.");
    auto j = nlohmann::json::parse(f);
    std::vector<Item> items;
    for (const auto& entry : j) {
        items.push_back(Item::fromJson(entry));
    }
    return items;
}

void ItemRepository::persist(const std::vector<Item>& items) const {
    nlohmann::json j = nlohmann::json::array();
    for (const auto& item : items) j.push_back(item.toJson());
    std::ofstream f(dataPath_);
    f << j.dump(2);
}

std::string ItemRepository::nextId(const std::vector<Item>& items) const {
    if (items.empty()) return "ITEM-001";
    int maxNum = 0;
    for (const auto& item : items) {
        int num = std::stoi(item.id.substr(5));
        if (num > maxNum) maxNum = num;
    }
    char buf[16];
    std::snprintf(buf, sizeof(buf), "ITEM-%03d", maxNum + 1);
    return buf;
}
