#include "Dungeon.h"
#include "Function.h"
#include "Dungeon_Reference.h"

using namespace std;

namespace overworld {

  Dungeon::Dungeon(const std::string &name, Scope &parent, const source_mapping::Source_Range source_point) :
//        element(Element_Type::other, name, *this, source_point),
    scope(&parent, Parent(*this)),
    name(name),
    self(new Dungeon_Reference(*this))
//        node(element, this, nullptr)
  {}

  Dungeon::Dungeon(const std::string &name) :
    scope(nullptr, Parent(*this)),
    name(name),
    self(new Dungeon_Reference(*this)) {}

  Function &Dungeon::get_or_create_constructor() {
    for (auto &method: scope.get_functions()) {
      if (method->get_name() == get_name())
        return *method;
    }
    throw std::runtime_error("Not implemented.");

//    return create_function(get_name(), *this, source_mapping::Source_Range());
//    throw std::runtime_error("Could not find constructor.");
  }

//  const Profession &Dungeon::get_profession() const {
//    return static_cast<Dungeon_Reference &>(*self);
//  }

//  Function &Dungeon::create_function(const std::string &name, Profession &profession,
//                                     const source_mapping::Source_Range &source_point) {
//    auto function = new Function_With_Block(name, profession, *this, source_point);
//    functions.push_back(unique_ptr<Function>(function));
////    if (!function->is_constructor())
////      graph.add_node(function->get_node());
//
//    add_member(name, Member(*function));
//    return *function;
//  }

  bool Dungeon::is_class() const {
    if (scope.get_minions().size() > 0)
      return true;

    if (scope.get_functions().size() > 0)
      return true;

    throw std::runtime_error("Not implemented.");
//    for (auto neighbor : node.get_neighbors()) {
//      auto &element = neighbor->get_element();
//      if (element.get_type() == Element_Type::instantiation)
//        return true;
//    }
    return false;
  }

  bool Dungeon::is_external() const {
    auto &external = Enchantment_Library::get_external();

    if (has_enchantment(external))
      return true;

//    auto parent = scope.get_parent_scope();
//    if (parent && parent->get_scope_type() == Scope_Type::dungeon) {
//      throw std::runtime_error("Not implemented.");
////      return dynamic_cast<Dungeon *>(parent)->has_enchantment(external);
//    }

    return false;
  }

  Dungeon &Dungeon::add_dungeon(std::unique_ptr<Dungeon> dungeon) {
    auto result = dungeon.get();
    scope.add_dungeon(std::move(dungeon));
    return *result;
  }

//  void Dungeon::add_enchantment(Enchantment &enchantment) {
//    enchantments.add_enchantment(enchantment);
//    if (&enchantment == &Enchantment_Library::get_value())
//      set_default_ownership(Ownership::value);
//  }

  Minion &Dungeon::get_minion(const std::string &name) {
    for (auto &variable: scope.get_minions()) {
      if (variable->get_name() == name)
        return *variable;
    }

    if (base_dungeon) {
      return base_dungeon->get_minion(name);
    }

    throw std::runtime_error("Could not find variable named " + name);
  }

  Member &Dungeon::get_member(const std::string &name) {
    if (scope.get_members().count(name) != 0)
      return scope.get_members().at(name);

    if (base_dungeon) {
      return base_dungeon->get_member(name);
    }

    throw std::runtime_error("Could not find member: " + name);
  }


  const std::string Dungeon::format_debug_name() const {
    if (generic_parameters.empty())
      return get_name();

    auto result = get_name() + "<";
    auto first = true;
    for (auto &parameter : generic_parameters) {
      if (first)
        first = false;
      else
        result += ", ";

      result += parameter->get_node().get_profession().get_name();
    }
    result += ">";

    return result;
  }

  const std::string get_namespace_string(const Parent &parent, const std::string &delimiter) {
    if (parent.get_type() == Parent_Type::dungeon) {
      auto &parent2 = parent.get_dungeon().get_original();
      if (parent2.get_name() != "") {
        return get_namespace_string(parent2.get_scope().get_parent(), delimiter)
               + delimiter + parent2.get_name();
      }
    }

    return "";
  }
}