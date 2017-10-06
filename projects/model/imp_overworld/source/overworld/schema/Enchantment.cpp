#include <overworld/expressions/Literal.h>
#include "Enchantment.h"
#include "Dungeon.h"

namespace overworld {

  const std::string Enchantment::get_name() const {
    return dungeon.get_name();
  }
  const std::string Enchantment::get_argument_string(int index) const {
    if (index >= 0 && index < arguments.size()) {
      auto literal_string = dynamic_cast<overworld::Literal_String *>(arguments[0].get());
      return literal_string->get_value();
    }
    return "";
  }
}