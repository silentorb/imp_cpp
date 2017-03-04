#pragma once
#include <memory>

namespace underworld {

  class Expression {

  };

  using Expression_Pointer = std:: unique_ptr<Expression>;
}