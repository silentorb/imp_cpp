#pragma once

#include <memory>
#include <underworld/expressions/Block.h>
#include "Member.h"
#include "Scope.h"

namespace underworld {

  class Parameter : public Minion {
  public:
      Parameter(const std::string &name, Profession_Owner profession, const source_mapping::Source_Point &source_point)
        : Minion(name, std::move(profession), source_point) {}

      bool is_parameter() const override {
        return true;
      }
  };

  class Virtual_Function : public Member {
      const std::string name;
      Profession_Owner return_type;
      bool _is_static = false;
      std::vector<std::string> generic_parameters;
      std::vector<std::unique_ptr<Parameter>> parameters;

  public:
      Virtual_Function(const std::string &name, Profession_Owner return_type,
                       const source_mapping::Source_Point &source,
                       Scope &parent) :
        name(name), return_type(std::move(return_type)), Member(source) {}

      Virtual_Function(const std::string &name, const source_mapping::Source_Point &source,
                       Scope &parent) :
        name(name), Member(source) {}

      Type get_type() const override {
        return Type::function;
      }

      const std::vector<std::unique_ptr<Parameter>> &get_parameters() const {
        return parameters;
      }

      const std::string get_name() const override {
        return name;
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

      void add_generic_parameter(const std::string &value) {
        generic_parameters.push_back(value);
      }

      const std::vector<std::string> &get_generic_parameters() const {
        return generic_parameters;
      }
  };

  class Function : public Virtual_Function {
      Block block;
      std::vector<Parameter *> parameters;

  public:
      Function(const std::string &name, Profession_Owner return_type, const source_mapping::Source_Point &source,
               Scope &parent) :
        Virtual_Function(name, std::move(return_type), source, parent), block(parent) {}

      Function(const std::string &name, const source_mapping::Source_Point &source,
               Scope &parent) :
        Virtual_Function(name, source, parent), block(parent) {}

      Block &get_block() {
        return block;
      }

      Parameter &add_parameter(const std::string &name, Profession_Owner profession,
                               const source_mapping::Source_Point &source);

      const std::vector<Parameter *> &get_parameters() const {
        return parameters;
      }

      const Block &get_block() const {
        return block;
      }
  };

  using Function_Owner = std::unique_ptr<Function>;
}