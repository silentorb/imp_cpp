#include <memory>
#include "Ingredient.h"
#include "Goblikin.h"
#include <vector>

void Goblikin::craft() {
  std::vector<std::unique_ptr<Ingredient>> ingredients;
  auto tail = std::unique_ptr<Ingredient>(new Ingredient());
  tail->name = "dragon's tail";
  ingredients.push_back(std::move(tail));
}