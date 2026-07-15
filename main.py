import sys
import io

sys.stdout = io.TextIOWrapper(sys.stdout.buffer, encoding="utf-8")
sys.stdin = io.TextIOWrapper(sys.stdin.buffer, encoding="utf-8")

from controller.item_controller import ItemController
from view.item_view import ItemView

MENU_ACTIONS = {
    "1": "create",
    "2": "list_all",
    "3": "detail",
    "4": "update",
    "5": "delete",
}


def run():
    controller = ItemController()
    view = ItemView()

    while True:
        choice = view.show_menu()

        if choice == "0":
            print("\n 시스템을 종료합니다.")
            break

        action = MENU_ACTIONS.get(choice)
        if action:
            getattr(controller, action)()
        else:
            view.show_error("올바른 메뉴 번호를 입력해주세요.")


if __name__ == "__main__":
    run()
