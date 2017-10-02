#pragma once

#include <memory>
#include <underworld/expressions/Block.h>
#include "Member.h"
#include "Scope.h"

namespace underworld {

  class Parameter : public Minion {
  public:
      Parameter(const std::string &name, Profession_Owner profession, const source_mapping::Source_Range &source_point)
        : Minion(name, std::move(profession), source_point) {}

      bool is_parameter() const override {
        return true;
      }
  };

  class Function : public Member {
      const std::string name;
      Profession_Owner return_type;
      bool _is_static = false;
      std::vector<std::string> generic_parameters;

  public:
      Function(const std::string &name, Profession_Owner return_type,
               const source_mapping::Source_Range &source,
               Scope &parent) :
        name(name), return_type(std::move(return_type)), Member(source) {}

      Function(const std::string &name, const source_mapping::Source_Range &source,
               Scope &parent) :
        name(name), Member(source) {}

      Type get_type() const override {
        return Type::function;
      }

      const std::string get_name() const override {
        return name;
      }

      const Profession *get_profession() const override {
        return return_type.get();
      }

      void add_generic_parameter(const std::string &value) {
        generic_parameters.push_back(value);
      }

      const std::vector<std::string> &get_generic_parameters() const {
        return generic_parameters;
      }

      virtual bool has_block() const {
        return false;
      }

  };

  class Virtual_Function : public Function {
      std::vector<std::unique_ptr<Parameter>> parameters;

  public:
      Virtual_Function(const std::string &name, Profession_Owner return_type,
                       const source_mapping::Source_Range &source, Scope &parent) :
        Function(name, std::move(return_type), source, parent) {}

      Virtual_Function(const std::string &name, const source_mapping::Source_Range &source, Scope &parent) :
        Function(name, source, parent) {}

      virtual void add_parameters(std::vector<std::unique_ptr<Parameter>> &new_parameters) {
        for (auto &parameter: new_parameters) {
          parameters.push_back(std::move(parameter));
        }
      }

      const std::vector<std::unique_ptr<Parameter>> &get_parameters() const {
        return parameters;
      }

      virtual Parameter &add_parameter(const std::string &name, Profession_Owner profession,
                                       const source_mapping::Source_Range &source);
  };

  class Function_With_Block : public Function {
      Block block;
      std::vector<Parameter *> parameters;

  public:
      Function_With_Block(const std::string &name, Profession_Owner return_type,
                          const source_mapping::Source_Range &source,
                          Scope &parent) :
        Function(name, std::move(return_type), source, parent), block(parent) {}

      Function_With_Block(const std::string &name, const source_mapping::Source_Range &source,
                          Scope &parent) :
        Function(name, source, parent), block(parent) {}

      Block &get_block() {
        return block;
      }

      void add_parameters(std::vector<std::unique_ptr<Parameter>> &new_parameters) {
        for (auto &parameter: new_parameters) {
          parameters.push_back(parameter.get());
          block.get_scope().add_member(std::move(parameter));
        }
      }

      Parameter &add_parameter(const std::string &name, Profession_Owner profession,
                               const source_mapping::Source_Range &source);

      const std::vector<Parameter *> &get_parameters() const {
        return parameters;
      }

      const Block &get_block() const {
        return block;
      }

      bool has_block() const override {
        return true;
      }
  };

  using Function_Owner = std::unique_ptr<Function>;
}