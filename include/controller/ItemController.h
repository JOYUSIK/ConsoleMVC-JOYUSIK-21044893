#pragma once
#include "repository/ItemRepository.h"
#include "view/ItemView.h"

class ItemController {
public:
    ItemController();

    void create();
    void listAll();
    void detail();
    void update();
    void remove();

private:
    ItemRepository repo_;
    ItemView view_;
};
