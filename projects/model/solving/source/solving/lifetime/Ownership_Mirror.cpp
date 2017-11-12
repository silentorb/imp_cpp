#include <overworld/expressions/Literal.h>
#include "Ownership_Mirror.h"

using namespace overworld;

namespace lifetime {
  Node &Ownership_Mirror::create_node(const Overworld_Element &profession_source, Lifetime_Ownership ownership) {
    auto node = new Node(profession_source, ownership);
    graph.add_node(Node_Owner(node));
    return *node;
  }

  Node &Ownership_Mirror::reflect_instantiation(overworld::Instantiation &instantiation) {
    return create_node(Overworld_Element(*instantiation.get_node()), Lifetime_Ownership::move);
  }

  void Ownership_Mirror::reflect_assignment(overworld::Assignment &assignment) {
    auto &target = reflect_expression(assignment.get_target());
    auto &value = reflect_expression(assignment.get_value());
    graph.connect(value, target);
  }

//  Node &Ownership_Mirror::reflect_variable_declaration(overworld::Minion_Declaration &variable_declaration,
//                                                       Context &context) {
//
//  }

  Node &Ownership_Mirror::reflect_member(overworld::Member_Expression &member_expression) {
    auto &member = member_expression.get_member();
    if (member.get_type() == Member_Type::minion)
      return graph.get_or_create_variable_node(member_expression.get_member().get_minion());

    if (member.get_type() == Member_Type::parameter)
      return graph.get_or_create_variable_node(member_expression.get_member().get_parameter());

    throw std::runtime_error("Not supported");
  }

  void Ownership_Mirror::reflect_variable_declaration_and_assignment(
    overworld::Minion_Declaration_And_Assignment &variable_declaration) {

  }

  Node &Ownership_Mirror::reflect_chain(overworld::Chain &chain) {
    return reflect_expression(chain.get_last());
  }

  Node &Ownership_Mirror::reflect_expression(Expression &expression) {
    auto type = expression.get_type();
    switch (expression.get_type()) {

      case Expression_Type::instantiation: {
        return reflect_instantiation(static_cast<Instantiation &>(expression));
        break;
      }

      case Expression_Type::member: {
        return reflect_member(static_cast<Member_Expression &>(expression));
        break;
      }

      case Expression_Type::variable_declaration: {
//        reflect_variable_declaration(static_cast<Minion_Declaration &>(expression));
        break;
      }

      case Expression_Type::chain: {
        return reflect_chain(static_cast<Chain &>(expression));
        break;
      }

      case Expression_Type::literal: {
        return create_node(Overworld_Element(*static_cast<Literal &>(expression).get_node()), Lifetime_Ownership::copy);
      }

      case Expression_Type::block:
      case Expression_Type::Else:
      case Expression_Type::If:
      case Expression_Type::lambda:
      case Expression_Type::Operator:
      case Expression_Type::range:
      case Expression_Type::return_nothing:
      case Expression_Type::return_with_value:
      case Expression_Type::self:
      default:
        return *reflect_expression_statement(expression);
    }
    throw std::runtime_error("Not implemented");
  }

  Node *Ownership_Mirror::reflect_expression_statement(overworld::Expression &expression) {
    auto type = expression.get_type();
    switch (expression.get_type()) {
      case Expression_Type::invoke: {
        return nullptr;
      }
    }

    throw std::runtime_error("Not implemented");
  }

  void Ownership_Mirror::reflect_statement(overworld::Expression &expression) {
    switch (expression.get_type()) {

      case Expression_Type::assignment: {
        reflect_assignment(static_cast<Assignment &>(expression));
        break;
      }

      case Expression_Type::variable_declaration_and_assignment: {
        reflect_variable_declaration_and_assignment(static_cast<Minion_Declaration_And_Assignment &>(expression));
        break;
      }
      default:
        reflect_expression_statement(expression);
    }
  }

  void Ownership_Mirror::reflect_block(overworld::Block &block) {
    for (auto &expression:  block.get_expressions()) {
      reflect_statement(*expression);
    }
  }

  void Ownership_Mirror::reflect_function(Function_With_Block &function) {
    reflect_block(function.get_block());
  }

  void Ownership_Mirror::reflect_dungeon(Dungeon &dungeon) {
    for (auto &function: dungeon.get_scope().get_functions()) {
      auto function_with_block = dynamic_cast<Function_With_Block *>(function.get());
      if (function_with_block) {
        reflect_function(*function_with_block);
      }
    }

    for (auto &child : dungeon.get_scope().get_dungeons()) {
      reflect_dungeon(*child);
    }
  }

  void Ownership_Mirror::reflect(Dungeon &overworld_root) {
    reflect_dungeon(overworld_root);
  }
}