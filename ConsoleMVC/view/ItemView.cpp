#include "view/ItemView.h"
#include <iostream>
#include <iomanip>

static const std::string SEP(60, '=');

std::string ItemView::showMenu() const {
    std::cout << "\n" << SEP << "\n";
    std::cout << " 아이템 관리 시스템\n";
    std::cout << SEP << "\n";
    std::cout << " [1] 아이템 등록\n";
    std::cout << " [2] 아이템 목록 조회\n";
    std::cout << " [3] 아이템 상세 조회\n";
    std::cout << " [4] 아이템 수정\n";
    std::cout << " [5] 아이템 삭제\n";
    std::cout << " [0] 종료\n";
    std::cout << SEP << "\n";
    std::cout << " 선택 > ";
    std::string choice;
    std::getline(std::cin, choice);
    return choice;
}

Item ItemView::inputItem() const {
    std::cout << "\n" << SEP << "\n 아이템 등록\n" << SEP << "\n";
    Item item;
    std::cout << " 이름        > "; std::getline(std::cin, item.name);
    std::cout << " 설명        > "; std::getline(std::cin, item.description);
    std::string qty;
    std::cout << " 수량        > "; std::getline(std::cin, qty);
    item.quantity = qty.empty() ? 0 : std::stoi(qty);
    return item;
}

std::string ItemView::inputId(const std::string& action) const {
    std::cout << "\n " << action << "할 아이템 ID > ";
    std::string id;
    std::getline(std::cin, id);
    return id;
}

Item ItemView::inputUpdate(const Item& current) const {
    std::cout << "\n" << SEP << "\n 아이템 수정 (" << current.id << ")\n" << SEP << "\n";

    Item updated = current;
    std::string input;

    std::cout << " 현재 이름: " << current.name << "\n";
    std::cout << " 새 이름 (변경 없으면 Enter) > ";
    std::getline(std::cin, input);
    if (!input.empty()) updated.name = input;

    std::cout << " 현재 설명: " << current.description << "\n";
    std::cout << " 새 설명 (변경 없으면 Enter) > ";
    std::getline(std::cin, input);
    if (!input.empty()) updated.description = input;

    std::cout << " 현재 수량: " << current.quantity << "\n";
    std::cout << " 새 수량 (변경 없으면 Enter) > ";
    std::getline(std::cin, input);
    if (!input.empty()) updated.quantity = std::stoi(input);

    return updated;
}

void ItemView::showItemList(const std::vector<Item>& items) const {
    std::cout << "\n" << SEP << "\n";
    std::cout << " 아이템 목록 (총 " << items.size() << "개)\n";
    std::cout << SEP << "\n";
    if (items.empty()) { std::cout << " 등록된 아이템이 없습니다.\n"; return; }

    std::cout << " " << std::left << std::setw(12) << "ID"
              << std::setw(16) << "이름"
              << std::setw(8)  << "수량"
              << "설명\n";
    std::cout << std::string(60, '-') << "\n";
    for (const auto& item : items) {
        std::cout << " " << std::left << std::setw(12) << item.id
                  << std::setw(16) << item.name
                  << std::setw(8)  << item.quantity
                  << item.description << "\n";
    }
}

void ItemView::showItemDetail(const Item& item) const {
    std::cout << "\n" << SEP << "\n 아이템 상세\n" << SEP << "\n";
    std::cout << " ID       : " << item.id          << "\n";
    std::cout << " 이름     : " << item.name        << "\n";
    std::cout << " 설명     : " << item.description << "\n";
    std::cout << " 수량     : " << item.quantity     << "\n";
    std::cout << " 등록일시 : " << item.createdAt   << "\n";
}

void ItemView::showSuccess(const std::string& msg) const {
    std::cout << "\n [완료] " << msg << "\n";
}

void ItemView::showError(const std::string& msg) const {
    std::cout << "\n [오류] " << msg << "\n";
}

bool ItemView::confirm(const std::string& msg) const {
    std::cout << "\n " << msg << " [Y/N] > ";
    std::string ans;
    std::getline(std::cin, ans);
    return !ans.empty() && (ans[0] == 'Y' || ans[0] == 'y');
}
