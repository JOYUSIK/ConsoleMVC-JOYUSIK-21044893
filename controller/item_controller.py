from model.item import Item
from repository.item_repository import ItemRepository
from view.item_view import ItemView


class ItemController:
    def __init__(self):
        self._repo = ItemRepository()
        self._view = ItemView()

    def create(self) -> None:
        raw = self._view.input_item()
        if not self._is_valid_input(raw):
            self._view.show_error("이름과 수량은 필수 입력 항목입니다.")
            return
        if not raw["quantity"].isdigit():
            self._view.show_error("수량은 숫자로 입력해주세요.")
            return

        item = Item(
            name=raw["name"],
            description=raw["description"],
            quantity=int(raw["quantity"]),
        )
        saved = self._repo.save(item)
        self._view.show_success(f"아이템이 등록되었습니다. (ID: {saved.id})")

    def list_all(self) -> None:
        items = self._repo.find_all()
        self._view.show_item_list(items)

    def detail(self) -> None:
        item_id = self._view.input_id("조회")
        item = self._repo.find_by_id(item_id)
        if not item:
            self._view.show_error(f"존재하지 않는 ID입니다: {item_id}")
            return
        self._view.show_item_detail(item)

    def update(self) -> None:
        item_id = self._view.input_id("수정")
        item = self._repo.find_by_id(item_id)
        if not item:
            self._view.show_error(f"존재하지 않는 ID입니다: {item_id}")
            return

        raw = self._view.input_update(item)
        item.name = raw["name"]
        item.description = raw["description"]
        item.quantity = raw["quantity"]

        self._repo.update(item)
        self._view.show_success(f"아이템이 수정되었습니다. (ID: {item.id})")

    def delete(self) -> None:
        item_id = self._view.input_id("삭제")
        item = self._repo.find_by_id(item_id)
        if not item:
            self._view.show_error(f"존재하지 않는 ID입니다: {item_id}")
            return

        self._view.show_item_detail(item)
        if not self._view.confirm("정말 삭제하시겠습니까?"):
            self._view.show_success("삭제가 취소되었습니다.")
            return

        self._repo.delete(item_id)
        self._view.show_success(f"아이템이 삭제되었습니다. (ID: {item_id})")

    def _is_valid_input(self, raw: dict) -> bool:
        return bool(raw.get("name")) and bool(raw.get("quantity"))
