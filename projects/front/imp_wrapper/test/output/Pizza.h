#pragma once

class Pizza {

    int size;

public:
    Pizza() {}

    std::string add_topping(int value);

    std::string crazy() {
      return add_topping(12);
    }
};