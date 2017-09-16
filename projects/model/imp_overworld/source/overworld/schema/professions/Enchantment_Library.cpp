#include "Enchantment_Library.h"

using namespace std;

namespace overworld {

  struct Static_Enchantments {
      Simple_Enchantment external = string("extern");
      Simple_Enchantment Static = string("static");
      Simple_Enchantment Public = string("public");
      Simple_Enchantment Private = string("private");
      Simple_Enchantment Protected = string("protected");
      Simple_Enchantment value = string("value");
  };

  static Static_Enchantments static_enchantments;

  Enchantment_Library::Enchantment_Library() {
    auto count = sizeof(Static_Enchantments) / sizeof(Simple_Enchantment);
    auto pointer = (Simple_Enchantment*) &static_enchantments;
    for (int i = 0; i < count; ++i) {
      auto enchantment = (pointer + i);
      global_enchantments[enchantment->get_name()] = enchantment;
    }
  }

  Enchantment *Enchantment_Library::find_enchantment(const std::string &name) {
    if (global_enchantments.count(name))
      return global_enchantments[name];

    return nullptr;
  }

  Simple_Enchantment &Enchantment_Library::get_external() {
    return static_enchantments.external;
  }

  Simple_Enchantment &Enchantment_Library::get_static() {
    return static_enchantments.Static;;
  }

  Simple_Enchantment &Enchantment_Library::get_public() {
    return static_enchantments.Public;
  }

  Simple_Enchantment &Enchantment_Library::get_private() {
    return static_enchantments.Private;
  }

  Simple_Enchantment &Enchantment_Library::get_protected() {
    return static_enchantments.Protected;
  }

  Simple_Enchantment &Enchantment_Library::get_value() {
    return static_enchantments.value;
  }
}