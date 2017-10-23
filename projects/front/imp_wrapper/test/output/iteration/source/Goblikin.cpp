#include <memory>
#include <iostream>
#include "Ingredient.h"
#include <vector>
#include "Goblikin.h"

void Goblikin::craft() {
  auto ingredients = std::vector<std::unique_ptr<Ingredient>>();
  auto tail = std::unique_ptr<Ingredient>(new Ingredient());
  tail->name = "dragon's tail";
  ingredients.push_back(std::move(tail));
  ingredients.map([&] () {
  std::cout(i.name);
});
}