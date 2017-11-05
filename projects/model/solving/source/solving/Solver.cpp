#include "Solver.h"
#include <algorithm>
#include <overworld/schema/Function.h>
#include <overworld/schema/professions/cloning.h>
#include <iostream>

using namespace overworld;

namespace solving {

  bool professions_match(const Profession &first, const Profession &second) {
    if (first.get_type() != second.get_type())
      return false;

    if (&first == &second) {
      return true;
    }

    switch (first.get_type()) {

      case Profession_Type::unknown:
      case Profession_Type::Void:
        return true;

      case Profession_Type::dungeon: {
        auto &first_dungeon = first.get_dungeon_interface();
        auto &second_dungeon = second.get_dungeon_interface();
        if (&first_dungeon == &second_dungeon)
          return true;

        if (first_dungeon.get_dungeon_type() == Dungeon_Type::variant
            && second_dungeon.get_dungeon_type() == Dungeon_Type::variant) {
          auto first_variant = static_cast<const Dungeon_Variant *>(&first_dungeon);
          auto second_variant = static_cast<const Dungeon_Variant *>(&second_dungeon);
          if (&first_variant->get_original() != &second_variant->get_original())
            return false;

          if (first_variant->get_arguments().size() != second_variant->get_arguments().size())
            throw std::runtime_error("Dungeon variants share same original but have different argument counts.");

          auto second_elements = second_variant->get_arguments().begin();
          for (auto &element : first_variant->get_arguments()) {
            if (!professions_match(*element->get_profession(), *(*second_elements++)->get_profession()))
              return false;
          }

          return true;
        }
        return false;
      }
        throw std::runtime_error("Not implemented.");

      case Profession_Type::function: {
        auto first_signature = static_cast<const Function_Signature *>(&first);
        auto second_signature = static_cast<const Function_Signature *>(&second);
        if (first_signature->get_elements().size() != second_signature->get_elements().size())
          return false;

        auto second_elements = second_signature->get_elements().begin();
        for (auto &element : first_signature->get_elements()) {
          if (!professions_match(*element->get_profession(), *(*second_elements++)->get_profession()))
            return false;
        }

        return true;
      }

      case Profession_Type::generic_parameter:
        throw std::runtime_error("Not implemented.");

      case Profession_Type::primitive:
        throw std::runtime_error("Not implemented.");

      case Profession_Type::reference:
        return professions_match(first.get_base(), second.get_base());


      case Profession_Type::Union:
        throw std::runtime_error("Not implemented.");
    }

    throw std::runtime_error("Not implemented.");
  }

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
    if (connection.get_type() != Connection_Type::direct) {
//      std::cout << "WARNING: Not yet checking conflicts across mapped connections." << std::endl;
      return false;
    }

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

    auto &previous_profession = node.get_profession().get_base(node.get_profession());

    if (previous_profession.get_type() == Profession_Type::temporary_interface) {
      auto temporary = static_cast<Temporary_Interface*>(previous_profession.get());
      temporary->replace(profession);
//      throw std::runtime_error("Not implemented.");
    }

    profession_library.assign(node, profession, setter);
    if (node.get_element().get_type() == Element_Type::minion
        && base_profession.get_type() == Profession_Type::generic_parameter) {
      auto &generic_parameter = *dynamic_cast<Generic_Parameter *>(base_profession.get());
      auto dungeon = dynamic_cast<Dungeon *>(&node.get_parent().get_dungeon());
      if (dungeon && !dungeon->has_generic_parameter(generic_parameter)) {
        auto &function = *dynamic_cast<Virtual_Function *>(&generic_parameter.get_node().get_parent().get_function());
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
    if (first.get_status() == Node_Status::unresolved)
      throw std::runtime_error("Should not be trying to push an unresolved node.");

    if (second.get_status() == Node_Status::resolved) {
#if DEBUG_SOLVER >= 2
      if (direction == Direction::out)
        std::cout << "  (" << (int) first.get_status() << ") " << first.get_debug_string()
                  << " !~> (" << (int) second.get_status() << ") " << second.get_debug_string() << std::endl;
      else
        std::cout << "  (" << (int) second.get_status() << ") " << second.get_debug_string()
                  << " <~! (" << (int) first.get_status() << ") " << first.get_debug_string() << std::endl;
#endif
      return 0;
    }

    if (second.get_status() == Node_Status::partial && first.get_status() == Node_Status::partial) {
#if DEBUG_SOLVER > 0
      std::cout << "# " << first.get_debug_string() << " :: " << second.get_debug_string() << std::endl;
#endif
      return 0;
    }

    auto profession = connection.get_profession(first);
    if (profession.get_type() == overworld::Profession_Type::unknown
        || profession.get_type() == Profession_Type::Void) {
      return 0;
    }

    if (professions_match(*profession, *second.get_profession())) {
#if DEBUG_SOLVER > 0
      std::cout << "  (" << (int) first.get_status() << ") " << first.get_debug_string() << " == "
                << "(" << (int) second.get_status() << ") " << second.get_debug_string() << std::endl;
#endif
      return 0;
    }

#if DEBUG_SOLVER > 0
    std::string operation = ">";
    if (connection.get_type() != Connection_Type::direct) {
      operation = &connection.get_first() == &first
                  ? "fs >"
                  : "sf >";
    }
    std::cout << "# (" << (int) first.get_status() << ") " << first.get_debug_string() << " " << operation << " "
              << "(" << (int) second.get_status() << ") " << second.get_debug_string() << std::endl;
#endif
    set_profession(second, profession);
    return 1;
  }

  Progress Solver::inhale(Node &node) {
    for (auto connection : node.get_connections()) {
      auto &other_node = connection->get_other(node);
      if (other_node.get_status() != Node_Status::unresolved) {
        if (try_push(other_node, node, *connection, Direction::in))
          return 1;
      }
    }
    return 0;
  }

  Progress Solver::exhale(Node &node) {
    Progress progress = 0;
    if (node.get_status() == Node_Status::unresolved)
      throw std::runtime_error("Should not being trying to push an unresolved node.");

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

  Profession_Reference *find_first_match(std::vector<Profession_Reference> &first_buffer,
                                         std::vector<Profession_Reference> &second_buffer) {
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
        variant_array, function, starting_node.get_parent().get_dungeon(), professions
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
      auto variant_tuple = Profession_Library::create_dungeon_variant(variant_array, dungeon, professions);
      clone_dungeon_graph(std::get<1>(variant_tuple), graph);
    }
  }

  void Solver::resolve_with_template_function(Connection &connection) {
    auto &first = connection.get_first();
    auto &second = connection.get_second();

    auto &function = first.get_parent().get_function().get_original();
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
    auto &parent = first.get_parent();
    if (parent.get_type() == Parent_Type::dungeon) {
      auto &dungeon = parent.get_dungeon();
      auto &variant_array = profession_library.get_dungeon_variant_array(dungeon);
      create_dungeon_variant(variant_array, dungeon.get_original(), first, second.get_profession());
    }
    else {
      auto &function = parent.get_function();
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
      if (node->get_status() != Node_Status::resolved &&
          node->get_status() != Node_Status::optional)
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
      if (node->get_status() != Node_Status::resolved
          && node->get_status() != Node_Status::optional) {
        return true;
      }
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

  void Solver::update_unresolved_without_void() {
    unresolved.clear();
    for (auto node : graph.get_nodes()) {
      if (node->get_status() != Node_Status::resolved
          && node->get_status() != Node_Status::optional) {
        unresolved.push_back(node);
      }
    }
  }

  bool Solver::_solve() {
    int x = 0;
    while (update_changed() || update_conflicts()) {
      auto progress = process_changed() +
                      process_conflicts();

      if (x++ > 20) {
        std::cout << "!!! Exceeded maximum solver iterations !!!" << std::endl;
        return false;
      }

      if (progress == 0) {
        if (has_unresolved_nodes() || !conflict_manager.conflicts.empty()) {
          return false;
        }
      }
    }

    return true;
  }

  bool Solver::solve() {
    initialize();
    bool result = _solve();
    return result;
  }
}