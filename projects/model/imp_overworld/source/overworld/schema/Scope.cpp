#include <underworld/schema/Minion.h>
#include <underworld/schema/Function.h>
#include "Scope.h"
#include "exceptions.h"
#include "Function.h"
#include "Dungeon.h"

using namespace std;

namespace overworld {

  Scope::Scope(Scope *parent) :
    parent(parent) {
  }

  Function_Scope::Function_Scope(Scope &parent, Function &function) :
    Scope(&parent), function(&function) {

  }

  Scope::~Scope() {
  }

  bool can_be_overloaded(Member &member) {
    if (member.get_type() == Member_Type::dungeon) {
      auto &dungeon = member.get_dungeon();
      return !dungeon.get_generic_parameters().empty();
    }
    else if (member.get_type() == Member_Type::function) {
      return true;
    }
    else {
      return false;
    }
  }

  void Scope::overload(const std::string &name, Member &member) {
    auto &existing = members.at(name);
    if (!can_be_overloaded(member) || member.get_type() != existing.get_type())
      throw std::runtime_error("Duplicated identifier: " + name + ".");

    overloaded_members[name] = Members();
    auto &array = overloaded_members[name];
    array.push_back(existing);
    array.push_back(member);
    members.erase(name);
  }

  void Scope::add_overload(const std::string &name, Member &member) {
    auto &array = overloaded_members.at(name);
    auto &first = array[0];

    if (!can_be_overloaded(member) || member.get_type() != first.get_type())
      throw std::runtime_error("Duplicated identifier: " + name + ".");

    array.push_back(member);
  }

  void Scope::add_member(const std::string &name, Member member) {
    if (members.count(name)) {
      overload(name, member);
    }
    else if (overloaded_members.count(name)) {
      add_overload(name, member);
    }
    else {
      members[name] = member;
    }
  }

  void Scope::add_function(std::unique_ptr<Function> function) {
    add_member(function->get_name(), Member(*function));
    functions.push_back(std::move(function));
  }

//  Member Scope::add_minion(Minion &minion) {
//    auto member = Member(minion);
//    add_member(minion.get_name(), Member(member));
//    return member;
//  }

  Member Scope::add_minion(std::unique_ptr<Minion> minion) {
    auto member = Member(*minion);
    add_member(minion->get_name(), member);
    minions.push_back(std::move(minion));
    return member;
  }

  Minion &Scope::get_minion(const std::string &name) {
    for (auto &variable: minions) {
      if (variable->get_name() == name)
        return *variable;
    }

    throw std::runtime_error("Could not find variable named " + name);
  }

//  void Scope::add_profession(std::unique_ptr<Profession> &profession) {
//    professions.push_back(std::move(profession));
//  }

  void Scope::add_profession(std::unique_ptr<Profession> &profession) {
    professions.push_back(std::move(profession));
  }

  void Scope::add_dungeon(std::unique_ptr<Dungeon> dungeon) {
    add_member(dungeon->get_name(), Member(*dungeon));
    dungeons.push_back(std::move(dungeon));
  }

  Dungeon &Scope::get_dungeon() {
    return parent->get_dungeon();
  }

  Parent Scope::get_parent() {
    auto function = get_function();
    return function
           ? Parent(*static_cast<Function_Interface *>(function))
           : Parent(dynamic_cast<Dungeon_Interface &>(get_dungeon()));
  }

  Function *Scope::get_function(const std::string &function_name) {
    for (auto &function: functions) {
      if (function->get_name() == function_name)
        return function.get();
    }

    return nullptr;
  }

//  Member *Scope::find_member(const std::string &name) {
//    if (members.count(name) != 0)
//      return members.at(name).get();
//
////    for (auto scope : imported_scopes) {
////      auto result = scope->get_member_or_null(name);
////      if (result)
////        return result;
////    }
//
//    if (parent)
//      return parent->find_member(name);
//
//    return nullptr;
//  }

  Member *Scope::get_member_or_null(const std::string &name) {
    return members.count(name) != 0
           ? &members.at(name)
           : nullptr;
  }

  Member &Scope::get_member(const std::string &name) {
    if (members.count(name) != 0)
      return members.at(name);

    throw std::runtime_error("Could not find member: " + name);
  }
}