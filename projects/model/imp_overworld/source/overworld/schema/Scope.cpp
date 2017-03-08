#include <underworld/schema/Minion.h>
#include <underworld/schema/Function.h>
#include "Scope.h"
#include "exceptions.h"
#include "Function.h"

using namespace std;

namespace overworld {
  Scope::Scope(const underworld::Scope &source) :
    source(source) {

    for (auto &input_member : source.get_members()) {
      if (input_member.second->get_type() == underworld::Member::Type::variable) {

      }
    }
  }

  Function &Scope::create_function(const underworld::Function &input) {
//    check_has_member(name);

    auto function = new Function(input);
    members[input.get_name()] = unique_ptr<Function>(function);
    return *function;
  }

  Minion &Scope::create_minion(const underworld::Minion &input, const Profession &profession) {
//    check_has_member(name);

    auto minion = new Minion(input, profession);
    members[input.get_name()] = unique_ptr<Minion>(minion);
    return *minion;
  }
}