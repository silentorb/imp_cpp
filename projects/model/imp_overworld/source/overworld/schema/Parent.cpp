#include <string>
#include "Parent.h"
#include "Dungeon.h"
#include "Function.h"

namespace overworld {

  const std::string Parent::get_name() const {
    if (type == Parent_Type::dungeon)
      return dungeon->get_original().get_name();

    if (type == Parent_Type::function)
      return function->get_original().get_name();

    throw std::runtime_error("Cannot get parent name.");
  }
}
