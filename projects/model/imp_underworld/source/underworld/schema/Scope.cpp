#include "Scope.h"
#include "underworld/exceptions.h"
#include "Function.h"
#include "Dungeon.h"

using namespace std;

namespace underworld {

  void Scope::throw_already_exists(const std::string &member_name) const {
    throw std::runtime_error("Local scope already has a member named " + member_name + ".");
  }

  Function &
  Scope::create_function(const std::string &member_name, const Profession &profession, const Source_Point &source) {
    auto function = new Function(member_name, profession, source);
    check_has_member(function->get_name());

    members[function->get_name()] = Function_Owner(function);
    return *function;
  }

  Minion &Scope::create_minion(const std::string &name, const Profession &profession, const Source_Point &source) {
    check_has_member(name);

    auto portal = new Minion(name, profession, source);
    members[name] = unique_ptr<Minion>(portal);
    return *portal;
  }

  Profession_Member &Scope::add_profession(std::unique_ptr<Profession> &profession, const Source_Point &source) {
    auto &prof = *profession;
    auto member = unique_ptr<Profession_Member>(new Profession_Member(profession, source));
    auto &result = *member;
    members[prof.get_name()] = std::move(member);
    return result;
  }

  Dungeon &Scope::create_dungeon(const std::string &name, const Source_Point &source) {
    auto dungeon = new Dungeon(name);
    auto pointer = unique_ptr<Profession>(dungeon);
    add_profession(pointer, source);
    return *dungeon;
  }

  Function *Scope::get_function(const std::string &name) const {
    if (members.count(name) == 0)
      return nullptr;

    auto &member = members.at(name);
    if (member->get_type() == Member::Type::function)
      return dynamic_cast<Function *>(member.get());

    return nullptr;
  }
}