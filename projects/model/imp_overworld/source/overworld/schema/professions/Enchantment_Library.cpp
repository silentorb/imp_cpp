#include <overworld/schema/Enchantment_With_Arguments.h>
#include "Enchantment_Library.h"
#include "professions.h"
#include "Profession_Library.h"

using namespace std;

namespace overworld {

  struct Enchantments {
      Dungeon Static;
      Dungeon Public;
      Dungeon Private;
      Dungeon Protected;
      Dungeon value;
      Dungeon external;
      Dungeon external_name;

      Enchantments() :
        Static("static"),
        Public("public"),
        Private("private"),
        Protected("protected"),
        value("value"),
        external("external"),
        external_name("external_name") {

      }
  };

//  struct Complex_Enchantments {
//      Dungeon external;
//      Dungeon external_name;
//
//      Complex_Enchantments() :
//        external("external", {
//          new Simple_Minion(
//            "header_file", Profession_Library::get_primitive(Primitive_Type::String)
//          )
//        }),
//        external_name("external_name", {
//          new Simple_Minion(
//            "name", Profession_Library::get_primitive(Primitive_Type::String)
//          )
//        }) {}
//  };
//
  static Enchantments simple_enchantments;
//  static std::unique_ptr<Complex_Enchantments> complex_enchantements;
//
//  void Enchantment_Library::initialize() {
//    if (!complex_enchantements)
//      complex_enchantements = std::unique_ptr<Complex_Enchantments>(new Complex_Enchantments());
//  }

  Enchantment_Library::Enchantment_Library() {
    auto count = sizeof(Enchantments) / sizeof(Dungeon);
    auto pointer = (Dungeon *) &simple_enchantments;
    for (int i = 0; i < count; ++i) {
      Dungeon *enchantment = (pointer + i);
      global_enchantments[enchantment->get_name()] = enchantment;
    }
  }

  Dungeon *Enchantment_Library::find_enchantment(const std::string &name) {
    if (global_enchantments.count(name))
      return global_enchantments[name];

    return nullptr;
  }

  Dungeon &Enchantment_Library::get_external() {
    return simple_enchantments.external;
  }

  Dungeon &Enchantment_Library::get_external_name() {
    return simple_enchantments.external_name;
  }

  Dungeon &Enchantment_Library::get_static() {
    return simple_enchantments.Static;;
  }

  Dungeon &Enchantment_Library::get_public() {
    return simple_enchantments.Public;
  }

  Dungeon &Enchantment_Library::get_private() {
    return simple_enchantments.Private;
  }

  Dungeon &Enchantment_Library::get_protected() {
    return simple_enchantments.Protected;
  }

  Dungeon &Enchantment_Library::get_value() {
    return simple_enchantments.value;
  }
}