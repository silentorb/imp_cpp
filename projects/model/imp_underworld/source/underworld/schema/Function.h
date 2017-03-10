#pragma once

#include <memory>
#include <underworld/expressions/Block.h>
#include "Member.h"
#include "Scope.h"

namespace underworld {

  class Function : public virtual Member {
      const std::string name;
      Block block;

  public:
      Function(const std::string &name) :
        name(name) {}

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
        return name;
      }
  };

  using Function_Owner = std::unique_ptr<Function>;
}