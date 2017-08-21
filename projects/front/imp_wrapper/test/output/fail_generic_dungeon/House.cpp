#include <House.h>

void House::set_inhabitant(std::unique_ptr<Princess> &value) {
  inhabitant = value;
  value->smile();
}