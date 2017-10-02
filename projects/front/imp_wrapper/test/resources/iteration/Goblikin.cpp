#include <memory>
#include "Ingredient.h"
#include <vector>
#include "Goblikin.h"

void Goblikin::craft() {
  std::vector<std::unique_ptr<Ingredient>> ingredients;
  auto tail = std::unique_ptr<Ingredient>(new Ingredient());
  tail->name = "dragon's tail";
  ingredients.push_back(std::move(tail));
}