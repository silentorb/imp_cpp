#pragma once

#include <memory>
#include <overworld/expressions/Block.h>
#include <underworld/schema/Function.h>
#include "Member.h"

namespace overworld {

  class Function : public virtual Member {
      const underworld::Function &source;
      Block block;
      Scope &parent_scope;
      std::vector<Minion *> parameters;

  public:
      Function(const underworld::Function &source, Scope &parent_scope) :
        source(source), block(source.get_block()), parent_scope(parent_scope) {}

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

      bool is_constructor() const;

      bool is_inline();

      const std::vector<Minion *> &get_parameters() const {
        return parameters;
      }

      void add_parameter(Minion & minion){
        parameters.push_back(&minion);
      }

  };

  using Function_Owner = std::unique_ptr<Function>;
}