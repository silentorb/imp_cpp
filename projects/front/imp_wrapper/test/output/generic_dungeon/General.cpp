#include "House.h"
#include "Frog.h"
#include <memory>
#include "Princess.h"
#include "General.h"

void General::main() {
  auto princess_house = std::unique_ptr<House>(new House());
  auto frog_house = std::unique_ptr<House>(new House());
  auto princess = std::unique_ptr<Princess>(new Princess());
  auto frog = std::unique_ptr<Frog>(new Frog());
  frog_house->set_inhabitant(std::move(frog));
  princess_house->set_inhabitant(std::move(princess));
}