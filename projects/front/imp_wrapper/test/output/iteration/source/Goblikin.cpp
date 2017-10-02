#include <string>
#include <memory>
#include "Ingredient.h"
#include <vector>
#include "Goblikin.h"

void Goblikin::craft() {
  auto ingredients = std::unique_ptr<std::vector>(new Array());
  auto tail = std::unique_ptr<Ingredient>(new Ingredient());
  tail->name = "dragon's tail";
  ingredients.push_back(std::move(tail));
}