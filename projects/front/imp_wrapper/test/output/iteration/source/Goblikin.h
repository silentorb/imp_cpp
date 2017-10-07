#pragma once

#include <memory>
#include <vector>

class Ingredient;

class Goblikin {
    std::vector<std::unique_ptr<Ingredient>> ingredients;
    std::unique_ptr<Ingredient> tail;

public:
    void craft();
};