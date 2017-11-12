#include <iostream>
#include "Ingredient.h"
#include <vector>
#include "Goblikin.h"

namespace iteration {

  void Goblikin::craft() {
    auto ingredients = std::vector<Ingredient>();
    auto tail = Ingredient();
    tail.name = "dragon's tail";
    ingredients.push_back(tail);

    for (auto &i : ingredients) {
      std::cout << i.name;
    }
  }
}