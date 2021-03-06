#include "Scope.h"
#include "underworld/exceptions.h"
#include "Function.h"
#include "Dungeon.h"

using namespace std;

namespace underworld {

  void Scope::throw_already_exists(const std::string &member_name) const {
    throw std::runtime_error("Local scope already has a member named " + member_name + ".");
  }

//  Function &Scope::create_function(const std::string &member_name, Profession_Owner &profession,
//                                   const source_mapping::Source_Range &source) {
//    auto function = new Function(member_name, std::move(profession), source, *this);
//    check_has_member(function->get_name());
//
//    members[function->get_name()] = Function_Owner(function);
//    return *function;
//  }

  void Scope::add_member(std::unique_ptr<Member> member) {
    check_has_member(member->get_name());
    members[member->get_name()] = std::move(member);
  }

  bool Scope::add_member2(std::unique_ptr<Member> member) {
    if (members.count(member->get_name()) != 0)
      return false;

    members[member->get_name()] = std::move(member);
    return true;
  }

  Minion &Scope::create_minion(const std::string &name, const source_mapping::Source_Range &source) {
    check_has_member(name);

    auto portal = new Minion(name, source);
    members[name] = unique_ptr<Minion>(portal);
    return *portal;
  }

//  Profession_Member &Scope::add_profession(unique_ptr<Profession> profession,
//                                           const source_mapping::Source_Range &source) {
//    auto &prof = *profession;
//    auto member = unique_ptr<Profession_Member>(new Profession_Member(profession, source));
//    auto &result = *member;
//    members[prof.get_name()] = std::move(member);
//    return result;
//  }

  Function *Scope::get_function(const std::string &name) const {
    if (members.count(name) == 0)
      return nullptr;

    auto &member = members.at(name);
    if (member->get_type() == Member::Type::function)
      return static_cast<Function *>(member.get());

    return nullptr;
  }

//  Profession_Member *Scope::get_profession(const std::string &name) const {
//    if (members.count(name) == 0)
//      return nullptr;
//
//    auto &member = members.at(name);
//    if (member->get_type() == Member::Type::profession)
//      return dynamic_cast<Profession_Member *>(member.get());
//
//    return nullptr;
//  }

  Dungeon &Scope::get_dungeon() {
    return parent->get_dungeon();
  }
}