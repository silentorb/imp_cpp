#pragma once

#include <vector>
#include "Expression.h"

namespace underworld {

  class Block {
      std::vector<Expression_Owner> expressions;
  };
}