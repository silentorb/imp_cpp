#include "Scope.h"
#include "exceptions.h"
#include "Function.h"

using namespace std;

namespace underworld {

  void Scope::throw_already_exists(const std::string &member_name) const {
    throw std::runtime_error("Local scope already has a member named " + member_name + ".");
  }

  Function &Scope::create_function(const std::string &name) {
    check_has_member(name);

    auto minion = new Function(name);
    members[name] = unique_ptr<Function>(minion);
    return *minion;
  }

  Minion &Scope::create_minion(const std::string &name, const Profession &profession) {
    check_has_member(name);

    auto portal = new Minion(name, profession);
    members[name] = unique_ptr<Minion>(portal);
    return *portal;
  }
}