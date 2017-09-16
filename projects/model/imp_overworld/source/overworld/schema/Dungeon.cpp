#include "Dungeon.h"
#include "Function.h"

using namespace std;

namespace overworld {

  Function &Dungeon::get_or_create_constructor() {
    for (auto &method: functions) {
      if (method->get_name() == get_name())
        return *method;
    }

    return create_function(get_name(), *this, source_mapping::Source_Point());
//    throw std::runtime_error("Could not find constructor.");
  }

  Function &Dungeon::create_function(const std::string &name, Profession &profession,
                                     const source_mapping::Source_Point &source_point) {
    auto function = new Function_With_Block(name, profession, *this, *this, source_point);
    functions.push_back(unique_ptr<Function>(function));
//    if (!function->is_constructor())
//      graph.add_node(function->get_node());

    add_member(name, Member_Owner(new Member_Function(*function)));
    return *function;
  }

  bool Dungeon::is_class() const {
    if (get_minions().size() > 0)
      return true;

    if (get_functions().size() > 0)
      return true;

    for (auto neighbor : node.get_neighbors()) {
      auto &element = neighbor->get_profession_reference();
      if (element.get_element_type() == Element_Type::instantiation)
        return true;
    }
    return false;
  }

  bool Dungeon::is_external() const {
    auto &external = Enchantment_Library::get_external();

    if (has_enchantment(external))
      return true;

    if (parent && parent->get_scope_type() == Scope_Type::dungeon) {
      return dynamic_cast<Dungeon *>(parent)->has_enchantment(external);
    }

    return false;
  }

  Dungeon &Dungeon::add_dungeon(std::unique_ptr<Dungeon> dungeon) {
    auto result = dungeon.get();
    Scope::add_dungeon(std::move(dungeon));
    return *result;
  }

  void Dungeon::add_enchantment(Enchantment &enchantment) {
    enchantments.add_enchantment(enchantment);
    if (&enchantment == &Enchantment_Library::get_value())
      set_default_ownership(Ownership::value);
  }

  Minion &Dungeon::get_minion(const std::string &name) {
    for (auto &variable: minions) {
      if (variable->get_name() == name)
        return *variable;
    }

    if (base_dungeon) {
      return base_dungeon->get_minion(name);
    }

    throw std::runtime_error("Could not find variable named " + name);
  }

  Member &Dungeon::get_member(const std::string &name) {
    if (members.count(name) != 0)
      return *members.at(name);

    if (base_dungeon) {
      return base_dungeon->get_member(name);
    }

    throw std::runtime_error("Could not find member: " + name);
  }


}