from dataclasses import dataclass, field
from datetime import datetime


@dataclass
class Item:
    name: str
    description: str
    quantity: int
    id: str = ""
    created_at: str = ""

    def __post_init__(self):
        if not self.created_at:
            self.created_at = datetime.now().strftime("%Y-%m-%dT%H:%M:%S")

    def to_dict(self) -> dict:
        return {
            "id": self.id,
            "name": self.name,
            "description": self.description,
            "quantity": self.quantity,
            "created_at": self.created_at,
        }

    @staticmethod
    def from_dict(data: dict) -> "Item":
        return Item(
            id=data["id"],
            name=data["name"],
            description=data["description"],
            quantity=data["quantity"],
            created_at=data["created_at"],
        )
