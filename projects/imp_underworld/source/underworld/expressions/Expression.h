#pragma once
#include <memory>

namespace underworld {

  class Expression {
public:

      enum class Type {
          block,
          literal
      };
  };

  using Expression_Owner = std:: unique_ptr<Expression>;
}