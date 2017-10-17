#include "Solver.h"
#include <algorithm>
#include <overworld/schema/Function.h>
#include <overworld/schema/professions/cloning.h>
#include <iostream>

using namespace overworld;

namespace solving {

  void Solver::add_node(Node &node) {
    if (node.get_status() != Node_Status::resolved)
      unresolved.push_back(&node);
  }

  void Solver::scan_fresh() {
    for (auto node : graph.get_nodes()) {
      add_node(*node);
    }
  }

  void Solver::add_conflict(Connection &connection) {
    for (auto conflict: conflict_manager.conflicts) {
      if (&conflict.get_connection() == &connection)
        return;
    }

    conflict_manager.conflicts.push_back({connection, false});
  }

  bool Solver::assignment_is_compatible(overworld::Profession &first, overworld::Profession &second) {
    return can_cast(first, second);
  }

  bool Solver::is_conflict(Connection &connection) {
    auto &first = connection.get_first();
    auto &second = connection.get_second();
    if (first.get_status() != Node_Status::resolved || second.get_status() != Node_Status::resolved)
      return false;

    if (!assignment_is_compatible(*first.get_profession(), *second.get_profession())) {
      return true;
    }

    return false;
  }

  void Solver::check_node_conflicts(Node &node) {
    for (auto connection: node.get_connections()) {
      if (is_conflict(*connection))
        add_conflict(*connection);
    }
  }

  void Solver::set_changed(Node &node) {
    node.set_changed(true);
    next_changed->push_back(&node);
    check_node_conflicts(node);
  }

  void Solver::set_profession(Node &node, overworld::Profession_Reference &profession) {
    auto &base_profession = profession.get_base(profession);

#if DEBUG_SOLVER > 0
    if (base_profession.get_type() == overworld::Profession_Type::unknown)
      throw std::runtime_error("Invalid type.");
#endif

    auto &original_profession = node.get_profession().get_base();
    profession_library.assign(node, profession, setter);
    if (node.get_element().get_type() == Element_Type::minion
        && base_profession.get_type() == Profession_Type::generic_parameter) {
      auto &generic_parameter = *dynamic_cast<Generic_Parameter *>(base_profession.get());
      auto dungeon = dynamic_cast<Dungeon *>(node.get_dungeon());
      if (dungeon && !dungeon->has_generic_parameter(generic_parameter)) {
        auto &function = *dynamic_cast<Virtual_Function *>(generic_parameter.get_node().get_function());
        migrate_generic_parameter_from_function_to_dungeon(*dungeon, function, generic_parameter);
      }
    }
//    if (node.get_profession_reference().get_element_type() == Element_Type::minion
//        && original_profession.get_type() == Profession_Type::generic_parameter) {
//      auto generic_parameter = dynamic_cast<Generic_Parameter *>(&original_profession);
//      auto dungeon = dynamic_cast<Dungeon *>(generic_parameter->get_node().get_dungeon());
//      if (dungeon) {
//        auto &variant_array = profession_library.get_dungeon_variant_array(*dungeon);
//        create_dungeon_variant(variant_array, dungeon->get_original(), node, base_profession);
//      }
//      else {
//        auto function = dynamic_cast<Function *>(generic_parameter->get_node().get_function());
//        migrate_generic_parameter_from_function_to_dungeon(*dungeon, *function, *generic_parameter);
////
//      }
//      if (dungeon && !dungeon->has_generic_parameter(*generic_parameter)) {
//    }
    set_changed(node);
  }

  void Solver::migrate_generic_parameter_from_function_to_dungeon(overworld::Dungeon &dungeon,
                                                                  overworld::Function &function,
                                                                  overworld::Generic_Parameter &parameter) {
    auto parameter_owner = function.detach_generic_parameter(parameter);
    dungeon.add_generic_parameter(std::move(parameter_owner));
  }

  Progress Solver::try_push(Node &first, Node &second, Connection &connection, Direction direction) {
    if (second.get_status() != Node_Status::resolved) {
#if DEBUG_SOLVER > 0
      std::cout << "# " << first.get_debug_string() << " > " << second.get_debug_string() << std::endl;
#endif
      auto &profession = first.get_profession();
      if (connection.get_type() == Connection_Type::direct) {
        set_profession(second, profession);
      }
      else {
        throw std::runtime_error("Not implemented.");
      }
      return 1;
    }
    else if (second.get_status() == Node_Status::partial && first.get_status() == Node_Status::partial) {
#if DEBUG_SOLVER > 0
      std::cout << "# " << first.get_debug_string() << " :: " << second.get_debug_string() << std::endl;
#endif
    }
#if DEBUG_SOLVER >= 2
    else {
      auto foo = first.get_debug_string();
      if (foo[foo.size() - 1] == ')') {
        first.get_status();
        auto k2 = foo + "bob";
        int k = 0;
      }
      if (direction == Direction::out)
        std::cout << "  " << first.get_debug_string() << " !~> " << second.get_debug_string() << std::endl;
      else
        std::cout << "  " << second.get_debug_string() << " <~! " << first.get_debug_string() << std::endl;
    }
#endif
    return 0;
  }

  Progress Solver::inhale(Node &node) {
    for (auto connection : node.get_connections()) {
      if (try_push(connection->get_other(node), node, *connection, Direction::in))
        return 1;
    }
    return 0;
  }

  Progress Solver::exhale(Node &node) {
    Progress progress = 0;
    for (auto connection : node.get_connections()) {
      progress += try_push(node, connection->get_other(node), *connection, Direction::out);
    }
    return progress;
  }

  Progress Solver::process_changed() {
    Progress progress = 0;
    for (auto node : *changed) {
      if (!node)
        continue;

      if (node->get_status() == Node_Status::resolved)
        progress += exhale(*node);
      else
        progress += inhale(*node);
    }
    return progress;
  }

  void get_ancestors_recursive(Profession &profession, std::vector<Profession_Reference> &buffer) {
    auto contracts_or_null = profession.get_contracts();
    if (!contracts_or_null)
      return;

    auto &contracts = *contracts_or_null;

    for (auto contract : contracts) {
      buffer.push_back(contract);
    }

    for (auto contract : contracts) {
      get_ancestors_recursive(*contract, buffer);
    }
  }

  void get_ancestors(Profession &profession, std::vector<Profession_Reference> &buffer) {
    buffer.clear();
    get_ancestors_recursive(profession, buffer);
  }

  bool buffer_has_item(std::vector<Profession_Reference> &buffer, Profession *item) {
    for (auto &entry: buffer) {
      if (entry.get() == item)
        return true;
    }

    return false;
  }

  Profession_Reference *find_first_match(std::vector<Profession_Reference> &first_buffer, std::vector<Profession_Reference> &second_buffer) {
    for (auto &first_step : first_buffer) {
      if (buffer_has_item(second_buffer, first_step.get()))
        throw std::runtime_error("Not sure if this is working right.");
//        return &first_step;
    }

    return nullptr;
  }

  Function_Variant &Solver::create_function_variant(Function_Variant_Array &variant_array, Function &function,
                                                    Node &starting_node, Profession_Reference &profession) {
    auto professions = to_professions(function.get_generic_parameters(), 1);
    professions.push_back(profession);
    auto variant = Profession_Library::get_function_variant(variant_array, function, professions);
    if (!variant) {
      variant = &Profession_Library::create_function_variant(
        variant_array, function, *starting_node.get_dungeon(), professions
      );
      clone_function_graph(*variant, starting_node, profession, graph);
    }

    return *variant;
  }

  void Solver::create_dungeon_variant(overworld::Dungeon_Variant_Array &variant_array, overworld::Dungeon &dungeon,
                                      Node &starting_node, overworld::Profession_Reference &profession) {
    auto professions = to_professions(dungeon.get_generic_parameters(), 1);
    professions.push_back(profession);
    auto variant = Profession_Library::get_dungeon_variant(variant_array, professions);
    if (!variant) {
      variant = &Profession_Library::create_dungeon_variant(
        variant_array, dungeon, professions
      );
      clone_dungeon_graph(*variant, graph);
    }
  }

  void Solver::resolve_with_template_function(Connection &connection) {
    auto &first = connection.get_first();
    auto &second = connection.get_second();

    auto &function = first.get_function()->get_original();
    auto &variant_array = profession_library.get_function_variant_array(function);
    if (variant_array.size() == 0) {
      auto &parameter = function.add_generic_parameter();
      set_profession(first, parameter);
      auto &first_profession = first.get_profession();
      create_function_variant(variant_array, function, first, first_profession.get_base(first_profession));
    }
    auto &second_profession = second.get_profession();
    create_function_variant(variant_array, function, first, second_profession.get_base(second_profession));
  }

  void Solver::resolve_with_template_dungeon(Connection &connection) {
    auto &first = connection.get_first();
    auto &second = connection.get_second();
    auto dungeon = first.get_dungeon();
    if (dungeon) {
      auto &variant_array = profession_library.get_dungeon_variant_array(*dungeon);
      create_dungeon_variant(variant_array, dungeon->get_original(), first, second.get_profession());
    }
    else {
      auto &function = *first.get_function();
      auto &variant_array = profession_library.get_function_variant_array(function);
      auto &second_profession = second.get_profession();
      create_function_variant(variant_array, function.get_original(), first,
                              second_profession.get_base(second_profession));
    }
  }

  bool Solver::resolve_conflict(Connection &connection) {
    auto &first = connection.get_first();
    auto &second = connection.get_second();
    auto &first_profession = first.get_profession();
    auto &second_profession = second.get_profession();

//    if (first.get_profession_reference().get_element_type() == Element_Type::parameter
//        && first.get_function() != second.get_function()
//        && first.get_profession().get_type() == Profession_Type::generic_parameter) {
//      resolve_with_existing_template_function(connection);
//      return true;
//    }

#if DEBUG_SOLVER > 0
    std::cout << "C " << first.get_debug_string() << " != " << second.get_debug_string() << std::endl;
#endif

    get_ancestors(*first_profession, ancestors_buffer1);
    get_ancestors(*second_profession, ancestors_buffer2);

    auto common_ancestor = find_first_match(ancestors_buffer1, ancestors_buffer2);
    if (common_ancestor) {
      set_profession(first, *common_ancestor);
      return true;
    }
    if (first_profession.get_type() == Profession_Type::generic_parameter) {
      resolve_with_template_dungeon(connection);
      return true;
    }

    return false;
  }

  Progress Solver::process_conflicts() {
    Progress progress = 0;
    auto i = conflict_manager.conflicts.begin();
    while (i != conflict_manager.conflicts.end()) {
      auto &conflict = *i;
      if (!conflict.attempted_resolution) {
        if (resolve_conflict(conflict.get_connection())) {
          ++progress;
          conflict_manager.conflicts.erase(i++);
          continue;
        }
        else {
          conflict.attempted_resolution = true;
        }
      }
      ++i;
    }
    return progress;
  }

  int Solver::update_unresolved() {
    unresolved.clear();
    for (auto node : graph.get_nodes()) {
      if (node->get_status() != Node_Status::resolved)
        unresolved.push_back(node);
    }

    return unresolved.size();
  }

  int Solver::update_changed() {
    changed->clear();

    // Flip buffers
    auto temp = changed;
    changed = next_changed;
    next_changed = temp;
    return changed->size();
  }

  bool Solver::has_unresolved_nodes() {
    for (auto node : graph.get_nodes()) {
      if (node->get_status() != Node_Status::resolved)
        return true;
    }

    return false;
  }

  int Solver::update_conflicts() {
    return conflict_manager.conflicts.size();
  }

  void Solver::on_add(Node &node) {
    set_changed(node);
  }

  void Solver::on_connect(Connection &connection) {
    set_changed(connection.get_first());
    set_changed(connection.get_second());
  }

  void Solver::on_remove(Node &node) {
    for (auto i = 0; i < changed->size(); ++i) {
      if ((*changed)[i] == &node) {
        (*changed)[i] = nullptr;
        break;
      }
    }
  }

  void Solver::initialize() {
    for (auto node : graph.get_nodes()) {
      if (node->get_status() != Node_Status::unresolved)
        set_changed(*node);
    }

#if DEBUG_SOLVER > 0
//    std::cout << "\nChanged Nodes:" << std::endl;
//
//    for (auto node : *next_changed) {
//      std::cout << "  " << node->get_debug_string() << std::endl;
//    }
#endif
  }

  bool Solver::solve() {
    initialize();

    while (update_changed() || update_conflicts()) {
      auto progress = process_changed() +
                      process_conflicts();

      if (progress == 0) {
        if (has_unresolved_nodes() || !conflict_manager.conflicts.empty())
          return false;
      }
    }

    return true;
  }
}