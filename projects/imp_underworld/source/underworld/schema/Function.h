#pragma once

#include <memory>
#include <underworld/expressions/Block.h>
#include "Member.h"
#include "Scope.h"

namespace underworld {

  class Function : public virtual Member {
      const std::string name;
      Block block;
      Scope scope;

  public:
      Function(const std::string &name) : name(name) {}

      Block &get_block() {
        return block;
      }

      Type get_type() const override {
        return Type::minion;
      }

      Scope &get_scope() {
        return scope;
      }

      const std::string &get_name() const {
        return name;
      }
  };

  using Minion_Pointer = std::unique_ptr<Function>;
}