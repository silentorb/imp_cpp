#include <Frog.h>
#include <Princess.h>
#include <General.h>

void General::main() {
  auto princess = std::unique_ptr<Princess>(new Princess());
  auto frog = std::unique_ptr<Frog>(new Frog());
  kiss(frog);
  kiss(princess);
}