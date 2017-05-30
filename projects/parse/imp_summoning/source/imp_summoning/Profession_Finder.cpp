#include "Profession_Finder.h"
#include <underworld/schema/Dungeon.h>

namespace imp_summoning {

  underworld::Profession *find_profession(underworld::Scope &scope, const runic_imp::Symbols &lexicon, Stream &input) {
    std::string last = "";
    auto current_scope = &scope;

    do {
      auto name = input.current().get_text();
      auto member = current_scope->get_profession(name);
      if (member) {
        auto &profession = member->get_profession();
        if (!input.peek().is(lexicon.dot)) {
          return &profession;
        }
        else {
          if (profession.get_type() != underworld::Profession::Type::dungeon)
            throw std::runtime_error(name + "is not a namespace or class.");

          current_scope = dynamic_cast<underworld::Dungeon *>(&profession);
          input.next();
          input.next();
          last = name;
          continue;
        }
      }
      else if (last != "") {
        throw std::runtime_error(last + " does not have a member named " + name + ".");
      }

      current_scope = current_scope->get_parent();
    } while (current_scope);

    return nullptr;
  }

//  void Profession_Finder::prepare_path_buffer(Stream &input) {
//    path_buffer.clear();
//    path_buffer.push_back(input.current().get_text());
//
//    while (input.peek().is(lexicon.dot)) {
//      input.next();
//      if (!input.peek().is(lexicon.identifier))
//        break;
//
//      path_buffer.push_back(input.next().get_text());
//      ++progress;
//    }
//
//  }

//  underworld::Profession *Profession_Finder::find(underworld::Scope &scope, Stream &input) {
////    progress = 0;
////    prepare_path_buffer(input);
////    current_string = &path_buffer[0];
//    return find(input.current().get_text(), scope);
//  }

//  const std::string Profession_Finder::get_failure_path() const {
//    std::string result = path_buffer[0];
//    for (int i = 1; i < progress + 1; ++i) {
//      result += "." + path_buffer[i];
//    }
//
//    return result;
//  }
}