#pragma once

#include <memory>
#include <underworld/expressions/Block.h>
#include "Member.h"
#include "Scope.h"

namespace underworld {

  class Parameter : public Minion {
  public:
      Parameter(const std::string &name, Profession_Owner &profession, const source_mapping::Source_Point &source_point)
        : Minion(name, profession, source_point) {}

      bool is_parameter() const override {
        return true;
      }
  };

  class Function : public Member {
      const std::string name;
      Block block;
      std::vector<Parameter *> parameters;
      Profession_Owner return_type;
      bool _is_static = false;

  public:
      Function(const std::string &name, Profession_Owner &return_type, const source_mapping::Source_Point &source, Scope &parent) :
        name(name), return_type(std::move(return_type)), Member(source), block(parent) {}

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

      Parameter &add_parameter(const std::string &name, Profession_Owner &profession, const source_mapping::Source_Point &source);

      const std::vector<Parameter *> &get_parameters() const {
        return parameters;
      }

      const Profession *get_profession() const override {
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