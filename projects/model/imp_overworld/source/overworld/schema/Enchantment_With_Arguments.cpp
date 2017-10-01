#include "Enchantment_With_Arguments.h"
#include <overworld/expressions/Literal.h>

namespace overworld {
  const std::string Enchantment_With_Arguments::get_argument_string(int index) const {
    if (index >= 0 && index < arguments.size()) {
      auto literal_string = dynamic_cast<overworld::Literal_String *>(arguments[0].get());
      return literal_string->get_value();
    }
    return "";
  }
}