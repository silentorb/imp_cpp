#pragma once

#include <memory>
#include <underworld/expressions/Block.h>
#include "Member.h"
#include "Scope.h"

namespace underworld {

  class Minion : public virtual Member {
      const std::string name;
      Block block;
      Scope scope;

  public:
      Minion(const std::string &name) : name(name) {}

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

  using Minion_Pointer = std::unique_ptr<Minion>;
}