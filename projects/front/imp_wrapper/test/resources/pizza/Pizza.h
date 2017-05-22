#pragma once
#include <string>

class Pizza {
    int size;

public:
    Pizza() {}

    std::string add_topping(int value);
    void crazy() {
      add_topping(12);
    }
};