import json
import os
from model.item import Item

DATA_FILE = "data/items.json"


class ItemRepository:
    def __init__(self):
        os.makedirs("data", exist_ok=True)
        if not os.path.exists(DATA_FILE):
            self._write([])

    def find_all(self) -> list[Item]:
        return [Item.from_dict(d) for d in self._read()]

    def find_by_id(self, item_id: str) -> Item | None:
        return next((Item.from_dict(d) for d in self._read() if d["id"] == item_id), None)

    def save(self, item: Item) -> Item:
        records = self._read()
        item.id = self._next_id(records)
        records.append(item.to_dict())
        self._write(records)
        return item

    def update(self, item: Item) -> bool:
        records = self._read()
        for i, record in enumerate(records):
            if record["id"] == item.id:
                records[i] = item.to_dict()
                self._write(records)
                return True
        return False

    def delete(self, item_id: str) -> bool:
        records = self._read()
        filtered = [r for r in records if r["id"] != item_id]
        if len(filtered) == len(records):
            return False
        self._write(filtered)
        return True

    def _read(self) -> list[dict]:
        with open(DATA_FILE, "r", encoding="utf-8") as f:
            return json.load(f)

    def _write(self, records: list[dict]) -> None:
        with open(DATA_FILE, "w", encoding="utf-8") as f:
            json.dump(records, f, ensure_ascii=False, indent=2)

    def _next_id(self, records: list[dict]) -> str:
        if not records:
            return "ITEM-001"
        last_num = max(int(r["id"].split("-")[1]) for r in records)
        return f"ITEM-{last_num + 1:03d}"
