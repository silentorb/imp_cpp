#include "Princess.h"
#include <memory>
#include "Frog.h"
#include "General.h"

void General::main() {
  auto princess = std::unique_ptr<Princess>(new Princess());
  auto frog = std::unique_ptr<Frog>(new Frog());
  kiss(frog);
  kiss(princess);
}