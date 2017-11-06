#include <memory>
#include <iostream>
#include "Ingredient.h"
#include <vector>
#include "Goblikin.h"

namespace iteration {

  void Goblikin::craft() {
    auto ingredients = std::unique_ptr<stl::vector<iteration::Ingredient>>(new stl::vector<iteration::Ingredient>());
    auto tail = std::unique_ptr<iteration::Ingredient>(new iteration::Ingredient());
    tail.name = "dragon's tail";
    ingredients.push_back(tail);

    for (auto &i : ingredients) {
      std::cout << i.name;
    }
  }
}