#pragma once

#include <string>

class Pizza {
    int size;
    void add_topping(int value);

    void crazy() {
      auto k = add_topping(12);
    }

public:
};