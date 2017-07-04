#include "Profession_Finder.h"
#include <underworld/schema/Dungeon.h>

namespace imp_summoning {

  underworld::Profession_Owner find_profession(underworld::Scope &scope, const runic_imp::Symbols &lexicon,
                                               Stream &input) {
    throw std:: runtime_error("Not implemented.");
  }
//    std::string last = "";
//    auto current_scope = &scope;
//
//    do {
//      auto name = input.current().get_text();
//      auto member = current_scope->get_profession(name);
//      if (member) {
//        auto &profession = member->get_profession();
//        if (!input.peek().is(lexicon.dot)) {
//          return &profession;
//        }
//        else {
//          if (profession.get_type() != underworld::Profession::Type::dungeon)
//            throw std::runtime_error(name + "is not a namespace or class.");
//
//          current_scope = dynamic_cast<underworld::Dungeon *>(&profession);
//          input.next();
//          input.next();
//          last = name;
//          continue;
//        }
//      }
//      else if (last != "") {
//        throw std::runtime_error(last + " does not have a member named " + name + ".");
//      }
//
//      current_scope = current_scope->get_parent();
//    } while (current_scope);
//
//    return nullptr;
//  }
}