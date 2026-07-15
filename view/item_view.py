from model.item import Item

SEPARATOR = "=" * 60


class ItemView:
    def show_menu(self) -> str:
        print(f"\n{SEPARATOR}")
        print(" 아이템 관리 시스템")
        print(SEPARATOR)
        print(" [1] 아이템 등록")
        print(" [2] 아이템 목록 조회")
        print(" [3] 아이템 상세 조회")
        print(" [4] 아이템 수정")
        print(" [5] 아이템 삭제")
        print(" [0] 종료")
        print(SEPARATOR)
        return input(" 선택 > ").strip()

    def input_item(self) -> dict:
        print(f"\n{SEPARATOR}")
        print(" 아이템 등록")
        print(SEPARATOR)
        name = input(" 이름        > ").strip()
        description = input(" 설명        > ").strip()
        quantity = input(" 수량        > ").strip()
        return {"name": name, "description": description, "quantity": quantity}

    def input_id(self, action: str) -> str:
        return input(f"\n {action}할 아이템 ID > ").strip()

    def input_update(self, item: Item) -> dict:
        print(f"\n{SEPARATOR}")
        print(f" 아이템 수정 ({item.id})")
        print(SEPARATOR)
        print(f" 현재 이름: {item.name}")
        name = input(" 새 이름 (변경 없으면 Enter) > ").strip() or item.name
        print(f" 현재 설명: {item.description}")
        description = input(" 새 설명 (변경 없으면 Enter) > ").strip() or item.description
        print(f" 현재 수량: {item.quantity}")
        quantity_str = input(" 새 수량 (변경 없으면 Enter) > ").strip()
        quantity = int(quantity_str) if quantity_str else item.quantity
        return {"name": name, "description": description, "quantity": quantity}

    def show_item_list(self, items: list[Item]) -> None:
        print(f"\n{SEPARATOR}")
        print(f" 아이템 목록 (총 {len(items)}개)")
        print(SEPARATOR)
        if not items:
            print(" 등록된 아이템이 없습니다.")
            return
        print(f" {'ID':<12} {'이름':<15} {'수량':>6}  설명")
        print("-" * 60)
        for item in items:
            print(f" {item.id:<12} {item.name:<15} {item.quantity:>6}  {item.description}")

    def show_item_detail(self, item: Item) -> None:
        print(f"\n{SEPARATOR}")
        print(" 아이템 상세")
        print(SEPARATOR)
        print(f" ID       : {item.id}")
        print(f" 이름     : {item.name}")
        print(f" 설명     : {item.description}")
        print(f" 수량     : {item.quantity}")
        print(f" 등록일시 : {item.created_at}")

    def show_success(self, message: str) -> None:
        print(f"\n [완료] {message}")

    def show_error(self, message: str) -> None:
        print(f"\n [오류] {message}")

    def confirm(self, message: str) -> bool:
        answer = input(f"\n {message} [Y/N] > ").strip().upper()
        return answer == "Y"
