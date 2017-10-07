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
    if (member.get_member_type() == Member_Type::dungeon) {
      auto dungeon_member = static_cast<Member_Dungeon *>(&member);
      auto &dungeon = dungeon_member->get_dungeon();
      return !dungeon.get_generic_parameters().empty();
    }
    else if (member.get_member_type() == Member_Type::function) {
      return true;
    }
    else {
      return false;
    }
  }

  void Scope::use_member_array(const std::string &name, Member_Owner member, Member_Owner &existing_member) {
    if (!can_be_overloaded(*member))
      throw std::runtime_error("Duplicated identifier: " + name + ".");

    if (existing_member->get_member_type() == Member_Type::array) {
      auto member_array = static_cast<Member_Reference_Array *>(existing_member.get());
      member_array->add_member(std::move(member));
    }
    else {
      if (!can_be_overloaded(*existing_member))
        throw std::runtime_error("Duplicated identifier: " + name + ".");

      auto member_array = new Member_Reference_Array();
      auto member_array_owner = Member_Owner(member_array);
      member_array->add_member(std::move(existing_member));
      member_array->add_member(std::move(member));
      members[name] = std::move(member_array_owner);
    }
  }

  void Scope::add_member(const std::string &name, Member_Owner member) {
    if (members.count(name)) {
      auto &existing_member = members[name];
      use_member_array(name, std::move(member), existing_member);
    }
    else {
      members[name] = std::move(member);
    }
  }

  void Scope::add_function(std::unique_ptr<Function> function) {
    add_member(function->get_name(), Member_Owner(new Member_Function(*function)));
    functions.push_back(std::move(function));
  }

  Member &Scope::add_minion(Minion &minion) {
    auto member = new Member_Minion(minion);
    add_member(minion.get_name(), Member_Owner(member));
    return *member;
  }

  Member &Scope::add_minion(std::unique_ptr<Minion> minion) {
    auto member = new Member_Minion(*minion);
    add_member(minion->get_name(), Member_Owner(member));
    minions.push_back(std::move(minion));
    return *member;
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
    add_member(dungeon->get_name(), Member_Owner(new Member_Dungeon(*dungeon)));
    dungeons.push_back(std::move(dungeon));
  }

  Dungeon &Scope::get_dungeon() {
    return parent->get_dungeon();
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
           ? members.at(name).get()
           : nullptr;
  }

  Member &Scope::get_member(const std::string &name) {
    if (members.count(name) != 0)
      return *members.at(name);

    throw std::runtime_error("Could not find member: " + name);
  }
}