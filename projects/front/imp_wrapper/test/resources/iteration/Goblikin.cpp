#include <iostream>
#include "Ingredient.h"
#include <vector>
#include <memory>
#include "Goblikin.h"

void Goblikin::craft() {
  auto ingredients = std::vector<std::unique_ptr<Ingredient>>();
  auto tail = std::unique_ptr<Ingredient>(new Ingredient());
  tail->name = "dragon's tail";
  ingredients.push_back(std::move(tail));

  for (auto &i : ingredients) {
    std::cout << i->name;
  }
}