#pragma once

#include <memory>
#include <underworld/expressions/Block.h>
#include "Member.h"
#include "Scope.h"

namespace underworld {

  class Function : public Member {
      const std::string name;
      Block block;
      std::vector<Minion *> parameters;
       Profession_Owner return_type;
      bool _is_static = false;

  public:
      Function(const std::string &name, Profession_Owner &return_type, const Source_Point &source, Scope &parent) :
        name(name), return_type(std:: move(return_type)), Member(source), block(parent) {}

      Block &get_block() {
        return block;
      }

      const Block &get_block() const {
        return block;
      }

      Type get_type() const override {
        return Type::function;
      }

      const std::string get_name() const override {
        return name;
      }

      Minion &add_parameter(const std::string &name, Profession_Owner &profession, const Source_Point &source);

      const std::vector<Minion *> &get_parameters() const {
        return parameters;
      }

      const Profession * get_profession() const override {
        return return_type.get();
      }

      bool is_static() const {
        return _is_static;
      }

      void set_is_static(bool value) {
        _is_static = value;
      }
  };

  using Function_Owner = std::unique_ptr<Function>;
}