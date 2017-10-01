#include <overworld/schema/Enchantment_With_Arguments.h>
#include "Enchantment_Library.h"
#include "professions.h"
#include "Profession_Library.h"

using namespace std;

namespace overworld {

  struct Simple_Enchantments {
      Simple_Enchantment Static = string("static");
      Simple_Enchantment Public = string("public");
      Simple_Enchantment Private = string("private");
      Simple_Enchantment Protected = string("protected");
      Simple_Enchantment value = string("value");
  };

  struct Complex_Enchantments {
      Enchantment_With_Parameters external;

      Complex_Enchantments() :
        external("extern", {
          new Simple_Minion(
            "header_file", Profession_Library::get_primitive(Primitive_Type::String)
          )
        }) {}
  };

  static Simple_Enchantments simple_enchanements;
  static std::unique_ptr<Complex_Enchantments> complex_enchanements;

  void Enchantment_Library::initialize() {
    if (!complex_enchanements)
      complex_enchanements = std::unique_ptr<Complex_Enchantments>(new Complex_Enchantments());
  }

  Enchantment_Library::Enchantment_Library() {
    initialize();
    auto count = sizeof(Simple_Enchantments) / sizeof(Simple_Enchantment);
    auto pointer = (Simple_Enchantment *) &simple_enchanements;
    for (int i = 0; i < count; ++i) {
      Simple_Enchantment *enchantment = (pointer + i);
      global_enchantments[enchantment->get_name()] = enchantment;
    }

    global_enchantments["extern"] = &complex_enchanements->external;
  }

  Enchantment *Enchantment_Library::find_enchantment(const std::string &name) {
    if (global_enchantments.count(name))
      return global_enchantments[name];

    return nullptr;
  }

  Enchantment_With_Parameters &Enchantment_Library::get_external() {
    return complex_enchanements->external;
  }

  Simple_Enchantment &Enchantment_Library::get_static() {
    return simple_enchanements.Static;;
  }

  Simple_Enchantment &Enchantment_Library::get_public() {
    return simple_enchanements.Public;
  }

  Simple_Enchantment &Enchantment_Library::get_private() {
    return simple_enchanements.Private;
  }

  Simple_Enchantment &Enchantment_Library::get_protected() {
    return simple_enchanements.Protected;
  }

  Simple_Enchantment &Enchantment_Library::get_value() {
    return simple_enchanements.value;
  }
}