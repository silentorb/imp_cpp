#pragma once

#include <memory>
#include <overworld/expressions/Block.h>
#include <underworld/schema/Function.h>
#include "Member.h"
#include "Scope.h"

namespace overworld {

  class Function : public virtual Member {
      const underworld::Function &source;
      Block block;
      Scope scope;

  public:
      Function(const underworld::Function &source) :
        source(source) {}

      Block &get_block() {
        return block;
      }

      Type get_type() const override {
        return Type::minion;
      }

      Scope &get_scope() {
        return scope;
      }
  };

  using Minion_Pointer = std::unique_ptr<Function>;
}