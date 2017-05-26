#pragma once

#include <memory>
#include <underworld/expressions/Block.h>
#include "Member.h"
#include "Scope.h"

namespace underworld {

  class Function : public virtual Member {
      const std::string name;
      Block block;
      std::vector<Minion *> parameters;
      Source_Point source;
      const Profession &return_type;

  public:
      Function(const std::string &name, const Profession &return_type, const Source_Point &source) :
        name(name), return_type(return_type), source(source) {}

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

      Minion &add_parameter(const std::string &name, const Profession &profession, const Source_Point &source);

      const std::vector<Minion *> &get_parameters() const {
        return parameters;
      }

      const Source_Point &get_source_point() const {
        return source;
      }

      const Profession &get_return_type() const {
        return return_type;
      }
  };

  using Function_Owner = std::unique_ptr<Function>;
}