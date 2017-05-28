#include <underworld/schema/Minion.h>
#include <underworld/schema/Function.h>
#include "Scope.h"
#include "exceptions.h"
#include "Function.h"
#include "Dungeon.h"

using namespace std;

namespace overworld {

  Scope::Scope(const underworld::Scope &source, Scope *parent) :
    source(source), parent(parent) {

  }

  Function_Scope::Function_Scope(const underworld::Scope &source, Scope &parent, Function &function) :
    Scope(source, &parent), function(&function) {

  }

  Scope::~Scope() {

  }

  Function &Scope::create_function(const underworld::Function &input, const Profession &profession,
                                   overworld::Graph &graph) {
//    check_has_member(name);
    auto function = new Function(input, profession, *this);
    functions.push_back(unique_ptr<Function>(function));
    if (!function->is_constructor())
      graph.add_node(function->get_node());

    return *function;
  }

  Minion &Scope::create_minion(const underworld::Minion &input, const Profession &profession, overworld::Graph &graph) {
//    check_has_member(name);

    auto minion = new Minion(input, profession);
    minions.push_back(unique_ptr<Minion>(minion));
    graph.add_node(minion->get_node());
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

  void Scope::add_dungeon(std::unique_ptr<Dungeon> &dungeon) {
    dungeons.push_back(std::move(dungeon));

  }
}