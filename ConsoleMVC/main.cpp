#include <iostream>
#include <unordered_map>
#include <functional>
#include <windows.h>
#include "controller/ItemController.h"
#include "view/ItemView.h"

int main() {
    SetConsoleOutputCP(CP_UTF8);
    SetConsoleCP(CP_UTF8);

    ItemController controller;
    ItemView view;

    const std::unordered_map<std::string, std::function<void()>> actions = {
        {"1", [&]{ controller.create();  }},
        {"2", [&]{ controller.listAll(); }},
        {"3", [&]{ controller.detail();  }},
        {"4", [&]{ controller.update();  }},
        {"5", [&]{ controller.remove();  }},
    };

    while (true) {
        std::string choice = view.showMenu();
        if (choice == "0") {
            std::cout << "\n 시스템을 종료합니다.\n";
            break;
        }
        auto it = actions.find(choice);
        if (it != actions.end()) {
            it->second();
        } else {
            view.showError("올바른 메뉴 번호를 입력해주세요.");
        }
    }
    return 0;
}
