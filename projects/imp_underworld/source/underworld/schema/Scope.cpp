#include "Scope.h"
#include "exceptions.h"
#include "Minion.h"

using namespace std;

namespace underworld {

//  Variable &Scope::create_variable(const std::string &name, const Profession &profession) {
//    if (variables.count(name) != 0)
//      throw Already_Exists_Error("Scope", "name");
//
//    auto variable = new Variable(name, profession);
//    variables[name] = Variable_Pointer(variable);
//    return *variable;;
//  }
  void Scope::throw_already_exists(const std::string &member_name) const {
    throw std::runtime_error("Local scope already has a member named " + member_name + ".");
  }

  Minion &Scope::create_minion(const std::string &name) {
    check_has_member(name);

    auto minion = new Minion(name);
    members[name] = unique_ptr<Minion>(minion);
    return *minion;
  }

  Portal &Scope::create_portal(const std::string &name, const Profession & profession) {
    check_has_member(name);

    auto portal = new Portal(name, profession);
    members[name] = unique_ptr<Portal>(portal);
    return *portal;
  }
}