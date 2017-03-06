#pragma once

#include <memory>

namespace underworld {

  class Expression {
  public:

      enum class Type {
          block,
          Else,
          If,
          literal,
          return_nothing,
          return_with_value,
          variable_declaration,
          variable_declaration_and_assignment
      };

      virtual Type get_type() = 0;
  };

  using Expression_Owner = std::unique_ptr<Expression>;
}