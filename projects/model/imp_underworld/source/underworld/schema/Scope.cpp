#include "Scope.h"
#include "underworld/exceptions.h"
#include "Function.h"

using namespace std;

namespace underworld {

  void Scope::throw_already_exists(const std::string &member_name) const {
    throw std::runtime_error("Local scope already has a member named " + member_name + ".");
  }

  Function &Scope::create_function(const std::string &member_name, const Profession &profession, const Source_Point & source) {
    auto function = new Function(member_name, profession, source);
    check_has_member(function->get_name());

    members[function->get_name()] = Function_Owner(function);
    return *function;
  }

  Minion &Scope::create_minion(const std::string &name, const Profession &profession, const Source_Point & source) {
    check_has_member(name);

    auto portal = new Minion(name, profession, source);
    members[name] = unique_ptr<Minion>(portal);
    return *portal;
  }
}