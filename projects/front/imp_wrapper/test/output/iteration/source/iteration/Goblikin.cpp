#include <iostream>
#include "Ingredient.h"
#include <vector>
#include "Goblikin.h"

namespace iteration {

  void Goblikin::craft() {
    auto ingredients = std::vector<std::unique_ptr<Ingredient>>();
    auto tail = std::unique_ptr<Ingredient>(new Ingredient());
    tail->name = "dragon's tail";
    ingredients.push_back(tail);
    ingredients.map([&] (Ingredient i) {
  std::cout << i.name;
});
  }
}