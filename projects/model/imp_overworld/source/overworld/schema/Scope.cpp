#include <underworld/schema/Minion.h>
#include <underworld/schema/Function.h>
#include "Scope.h"
#include "Function.h"

using namespace std;

namespace overworld {

  Scope::Scope(Scope *parent, Parent owner) :
    parent_scope(parent), owner(owner) {
  }

  Scope::~Scope() {
  }

  void Scope::add_function(std::unique_ptr<Function> function) {
    add_member(function->get_name(), Member(*function));
    functions.push_back(std::move(function));
  }

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

  void Scope::add_profession(std::unique_ptr<Profession> &profession) {
    professions.push_back(std::move(profession));
  }

  void Scope::add_dungeon(std::unique_ptr<Dungeon> dungeon) {
    add_member(dungeon->get_name(), Member(*dungeon));
    dungeons.push_back(std::move(dungeon));
  }

  Function *Scope::get_function(const std::string &function_name) {
    for (auto &function: functions) {
      if (function->get_name() == function_name)
        return function.get();
    }

    return nullptr;
  }


}