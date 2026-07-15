#include "controller/ItemController.h"

ItemController::ItemController() : repo_(), view_() {}

void ItemController::create() {
    Item item = view_.inputItem();
    if (item.name.empty()) {
        view_.showError("이름은 필수 입력 항목입니다.");
        return;
    }
    Item saved = repo_.save(item);
    view_.showSuccess("아이템이 등록되었습니다. (ID: " + saved.id + ")");
}

void ItemController::listAll() {
    view_.showItemList(repo_.findAll());
}

void ItemController::detail() {
    std::string id = view_.inputId("조회");
    auto item = repo_.findById(id);
    if (!item) { view_.showError("존재하지 않는 ID입니다: " + id); return; }
    view_.showItemDetail(*item);
}

void ItemController::update() {
    std::string id = view_.inputId("수정");
    auto item = repo_.findById(id);
    if (!item) { view_.showError("존재하지 않는 ID입니다: " + id); return; }

    Item updated = view_.inputUpdate(*item);
    repo_.update(updated);
    view_.showSuccess("아이템이 수정되었습니다. (ID: " + updated.id + ")");
}

void ItemController::remove() {
    std::string id = view_.inputId("삭제");
    auto item = repo_.findById(id);
    if (!item) { view_.showError("존재하지 않는 ID입니다: " + id); return; }

    view_.showItemDetail(*item);
    if (!view_.confirm("정말 삭제하시겠습니까?")) {
        view_.showSuccess("삭제가 취소되었습니다.");
        return;
    }
    repo_.remove(id);
    view_.showSuccess("아이템이 삭제되었습니다. (ID: " + id + ")");
}
