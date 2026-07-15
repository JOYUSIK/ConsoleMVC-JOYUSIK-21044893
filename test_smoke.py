"""MVC 각 레이어 동작 검증용 스모크 테스트"""
import os
import sys
import json
import tempfile

os.chdir(os.path.dirname(os.path.abspath(__file__)))

from model.item import Item
from repository.item_repository import ItemRepository


def test_model():
    item = Item(name="테스트아이템", description="설명", quantity=10)
    assert item.name == "테스트아이템"
    assert item.quantity == 10
    assert item.created_at != ""

    d = item.to_dict()
    restored = Item.from_dict(d)
    assert restored.name == item.name
    print("  [PASS] Model: to_dict / from_dict 정상")


def test_repository_crud():
    import repository.item_repository as repo_module
    original_path = repo_module.DATA_FILE

    with tempfile.TemporaryDirectory() as tmpdir:
        repo_module.DATA_FILE = os.path.join(tmpdir, "items.json")
        with open(repo_module.DATA_FILE, "w") as f:
            json.dump([], f)

        repo = ItemRepository()

        item = Item(name="아이템A", description="desc", quantity=5)
        saved = repo.save(item)
        assert saved.id == "ITEM-001"
        print("  [PASS] Repository: save 정상")

        found = repo.find_by_id("ITEM-001")
        assert found is not None
        assert found.name == "아이템A"
        print("  [PASS] Repository: find_by_id 정상")

        found.quantity = 99
        repo.update(found)
        updated = repo.find_by_id("ITEM-001")
        assert updated.quantity == 99
        print("  [PASS] Repository: update 정상")

        repo.delete("ITEM-001")
        assert repo.find_by_id("ITEM-001") is None
        print("  [PASS] Repository: delete 정상")

        all_items = repo.find_all()
        assert len(all_items) == 0
        print("  [PASS] Repository: find_all 정상")

    repo_module.DATA_FILE = original_path


def test_id_sequence():
    import repository.item_repository as repo_module
    with tempfile.TemporaryDirectory() as tmpdir:
        repo_module.DATA_FILE = os.path.join(tmpdir, "items.json")
        with open(repo_module.DATA_FILE, "w") as f:
            json.dump([], f)

        repo = ItemRepository()
        for i in range(3):
            repo.save(Item(name=f"item{i}", description="", quantity=i))

        items = repo.find_all()
        ids = [item.id for item in items]
        assert ids == ["ITEM-001", "ITEM-002", "ITEM-003"]
        print("  [PASS] Repository: ID 시퀀스 정상 (ITEM-001 ~ ITEM-003)")


if __name__ == "__main__":
    print("\n=== MVC 스모크 테스트 ===")
    test_model()
    test_repository_crud()
    test_id_sequence()
    print("\n모든 테스트 통과!")
