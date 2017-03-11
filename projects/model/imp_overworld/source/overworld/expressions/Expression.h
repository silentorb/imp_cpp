#pragma once

#include <memory>
#include <vector>

namespace overworld {

  class Expression {
  public:

      enum class Type {
          assignment,
          block,
          Else,
          If,
          literal,
          minion,
          Operator, // Infix
          return_nothing,
          return_with_value,
          variable_declaration,
          variable_declaration_and_assignment
      };

      virtual Type get_type() const = 0;
  };

  using Expression_Owner = std::unique_ptr<Expression>;
  using Expressions = std::vector<Expression_Owner>;
}