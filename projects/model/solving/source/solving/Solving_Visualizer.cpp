#include "Solving_Visualizer.h"
#include <iostream>
#include <solving/lifetime/Graph.h>

using namespace overworld;

namespace solving {

  const std::string render_node_status(Node_Status status) {
    switch (status) {

      case Node_Status::unresolved:
        return "?";

      case Node_Status::partial:
        return "!?";

      default:
        return "";
    }
  }

  const std::string render_ownership(const Profession_Reference &profession) {
    if (profession.get_type() == Profession_Type::unknown
        || profession.get_type() == Profession_Type::Void)
      return "";

    switch (profession.get_ownership()) {
      case Ownership::value:
        return "+";

      case Ownership::reference:
        return "&";

      case Ownership::pointer:
        return "*";

      case Ownership::owner:
        return "$";

      default:
        return "?";
    }
  }

  const std::string get_node_debug_string(const Node &node) {
    auto &element = node.get_element();
    auto &profession = node.get_profession();
    auto &source_point = element.get_source_point().get_start();

    std::string result = "";
    if (source_point.get_source_file())
      result += //source_point.get_source_file()->get_short_file_path() + " " +
        std::to_string(source_point.get_row()) + ":" +
        std::to_string(source_point.get_column()) + " ";

    result += node.get_debug_string() + render_node_status(node.get_status())
              + ":"
              + render_ownership(profession)
              + profession.get()->get_debug_name();

    if (profession.get_type() == Profession_Type::reference)
      result += dynamic_cast<const Reference *>(profession.get())->is_pointer() ? "*" : "&";

    return result;
  }

  unsigned int get_row(overworld::Node &node) {
    return node.get_element().get_source_point().get_start().get_row();
  }

  bool source_point_is_before(const source_mapping::Source_Point &first, const source_mapping::Source_Point &second) {
    if (first.get_source_file() < second.get_source_file())
      return true;

    if (first.get_row() < second.get_row())
      return true;

    if (first.get_row() == second.get_row())
      return first.get_column() < second.get_column();

    return false;
  }

  template<typename Node>
  void insert_node(std::vector<Node *> &nodes, Node *node) {
    for (auto i = nodes.begin(); i != nodes.end(); i++) {
      auto first = node->get_element().get_source_point().get_start();
      auto second = (*i)->get_element().get_source_point().get_start();
      if (source_point_is_before(first, second)) {
        nodes.insert(i, node);
        return;
      }
    }

    nodes.push_back(node);
  }

  void log_node(overworld::Node &node) {
    std::cout << get_node_debug_string(node);
    std::cout << std::endl;
  }

  Node *find_node(Graph &graph, source_mapping::Measurement row, source_mapping::Measurement column) {
    for (auto node: graph.get_nodes()) {
      auto source_point = node->get_element().get_source_point().get_start();
      if (source_point.get_row() == row && source_point.get_column() == column)
        return node;
    }
    return nullptr;
  }

  std::vector<Node *> find_nodes(Graph &graph, source_mapping::Measurement row, source_mapping::Measurement column) {
    std::vector<Node *> result;
    for (auto node: graph.get_nodes()) {
      auto source_point = node->get_element().get_source_point().get_start();
      if (source_point.get_row() == row && source_point.get_column() == column) {
        result.push_back(node);
      }
    }
    return result;
  }

  std::string get_connection_symbol(Connection_Type type) {
    switch (type) {

      case Connection_Type::compound_to_scalar:
        return "V*";

      case Connection_Type::variant_to_lambda:
        return "LV";

      case Connection_Type::lambda_to_parameter:
        return "L*";

      default:
        return "**";
    }
    throw std::runtime_error("Not supported.");
  }

  void log_nodes(Graph &graph, Node_Info node_info) {
    std::cout << std::endl << "Logging nodes:" << std::endl;

    std::vector<overworld::Node *> nodes;
    for (auto node : graph.get_nodes()) {
      insert_node(nodes, node);
    }
    for (auto &node : nodes) {
//      log_node(*node);
//      std::cout << get_node_debug_string(*node);
      std::cout << node_info(*node) << std::endl;
      for (auto &connection : node->get_connections()) {
        std::cout << std::string("  ") + get_connection_symbol(connection->get_type()) + " ";
        log_node(connection->get_other(*node));
      }
    }

    std::cout << std::endl;
  }

  bool is_root(lifetime::Node &node) {
    for (auto connection : node.get_connections()) {
      if (&connection->get_second() == &node)
        return false;
    }
    return true;
  }

  const std::string render_ownership2(lifetime::Lifetime_Ownership ownership) {
    switch (ownership) {
      case lifetime::Lifetime_Ownership::unknown:
        return "unknown";

      case lifetime::Lifetime_Ownership::anchor:
        return "anchor";

      case lifetime::Lifetime_Ownership::copy:
        return "copy";

      case lifetime::Lifetime_Ownership::move:
        return "move";

      case lifetime::Lifetime_Ownership::implicit_move:
        return "implicit_move";

      case lifetime::Lifetime_Ownership::reference:
        return "reference";
    }

    throw std::runtime_error("Not supported.");
  }

  const std::string get_node_debug_string2(lifetime::Node &node) {
    auto &element = node.get_element().get_element();
//    auto &profession = node.get_profession();
    auto &source_point = element.get_source_point().get_start();

    std::string result = "";
    if (source_point.get_source_file())
      result += //source_point.get_source_file()->get_short_file_path() + " " +
        std::to_string(source_point.get_row()) + ":" +
        std::to_string(source_point.get_column()) + " ";

    result += node.get_element().get_debug_string()
      + " | " + render_ownership2(node.get_ownership());
//              + render_node_status(node.get_status())
//              + ":"
//              + render_ownership(profession)
//              + profession.get()->get_debug_name();

//    if (profession.get_type() == Profession_Type::reference)
//      result += dynamic_cast<const Reference *>(profession.get())->is_pointer() ? "*" : "&";

    return result;
  }

  void log_node_recursive(lifetime::Node &node, const std::string &indent = "") {
    if (indent.size() > 2 * 20)
      throw std::runtime_error("Infinite loop.");

    std::cout << indent << get_node_debug_string2(node) << std::endl;

    for (auto &connection : node.get_connections()) {
      if (&connection->get_first() == &node) {
        log_node_recursive(connection->get_other(node), indent + "  ");
      }
    }
  }

  void log_node_trees(lifetime::Graph &graph) {
    std::cout << std::endl << "Logging nodes:" << std::endl;

    std::vector<lifetime::Node *> nodes;
    for (auto &node : graph.get_nodes()) {
      if (is_root(*node))
        insert_node(nodes, node.get());
    }

    for (auto &node : nodes) {
      log_node_recursive(*node);
    }

    std::cout << std::endl;
  }
}