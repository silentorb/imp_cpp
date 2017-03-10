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

  public:
      Function(const underworld::Function &source) :
        source(source), block(source.get_block()) {}

      Block &get_block() {
        return block;
      }

      const Block &get_block() const {
        return block;
      }

      Type get_type() const override {
        return Type::function;
      }

      const std::string &get_name() const {
        return source.get_name();
      }

  };

  using Minion_Pointer = std::unique_ptr<Function>;
}