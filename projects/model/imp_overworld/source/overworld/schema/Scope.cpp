#include <underworld/schema/Minion.h>
#include <underworld/schema/Function.h>
#include "Scope.h"
#include "exceptions.h"
#include "Function.h"
#include "Dungeon.h"

using namespace std;

namespace overworld {

  Scope::Scope(const underworld::Scope *source, Scope *parent) :
    source(source), parent(parent) {
    int k = 0;
  }

  Scope::Scope(Scope *parent) :
    source(nullptr), parent(parent) {
  }

  Function_Scope::Function_Scope(Scope &parent, Function &function) :
    Scope(&parent), function(&function) {

  }

  Scope::~Scope() {
    int k = 0;
  }

  Function &Scope::create_function(const std::string &name, const Profession &profession,
                                   const underworld::Source_Point &source_point) {
    auto function = new Function(name, profession, *this, source_point);
    functions.push_back(unique_ptr<Function>(function));
//    if (!function->is_constructor())
//      graph.add_node(function->get_node());

    members[name] = function;
    return *function;
  }

  Function &Scope::create_function(const underworld::Function &input, const Profession &profession) {
    return create_function(input.get_name(), profession, input.get_source_point());
  }

  Function &Scope::create_function(const underworld::Function &input) {
    auto function = new Function(input.get_name(), *this, input.get_source_point());
    functions.push_back(unique_ptr<Function>(function));
    members[input.get_name()] = function;
    return *function;
  }

  void Scope::add_minion(Minion *minion) {
    minions.push_back(unique_ptr<Minion>(minion));
    members[minion->get_name()] = minion;
  }

  void Scope::add_minion(std::unique_ptr<Minion> &minion) {
    members[minion->get_name()] = minion.get();
    minions.push_back(std::move(minion));
  }

  Minion &Scope::create_minion(const underworld::Minion &input, const Profession &profession, overworld::Graph &graph) {
//    check_has_member(name);

    auto minion = new Minion(input, profession);
    minions.push_back(unique_ptr<Minion>(minion));
//    graph.add_node(minion->get_node());
    members[minion->get_name()] = minion;
    return *minion;
  }

//  Minion &Scope::create_minion(const underworld::Minion &input) {
//    auto minion = new Minion(input);
//    add_minion(minion);
//    return *minion;
//  }

  Minion &Scope::create_minion(const std::string &name, const Profession &profession) {
    auto minion = new Minion(name, profession);
    add_minion(minion);
    return *minion;
  }

  Minion &Scope::get_minion(const std::string &name) {
    for (auto &variable: minions) {
      if (variable->get_name() == name)
        return *variable;
    }

    throw std::runtime_error("Could not find variable named " + name);
  }

  void Scope::add_profession(std::unique_ptr<const Profession> &profession) {
    professions.push_back(std::move(profession));
  }

  void Scope::add_profession(std::unique_ptr<Profession> &profession) {
    professions.push_back(std::move(profession));
  }

  void Scope::add_dungeon(std::unique_ptr<Dungeon> &dungeon) {
    members[dungeon->get_name()] = dungeon.get();
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

  Member *Scope::find_member(const std::string &name) {
    return members.count(name) != 0
           ? members.at(name)
           : (parent
              ? parent->find_member(name)
              : nullptr);
  }

  Member *Scope::get_member(const std::string &name) {
    return members.count(name) != 0
           ? members.at(name)
           : nullptr;
  }
}