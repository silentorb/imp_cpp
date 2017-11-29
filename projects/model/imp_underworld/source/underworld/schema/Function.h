#pragma once

#include <memory>
#include <underworld/expressions/Block.h>
#include "Member.h"
#include "Scope.h"
#include "Generic_Parameter.h"
#include "Function_Signature.h"

namespace underworld {

  class Function : public Member {
      Function_Signature signature;
      const std::string name;
      std::vector<Generic_Parameter> generic_parameters;

  public:
      Function(const std::string &name,
               const source_mapping::Source_Range &source,
               Scope &parent) :
        name(name), Member(source) {}

    virtual ~Function() {
    }
      Type get_type() const override {
        return Type::function;
      }

      const std::string get_name() const override {
        return name;
      }

      const Function_Signature &get_signature() const {
        return signature;
      }

      void add_generic_parameter(Generic_Parameter &value) {
        generic_parameters.push_back(value);
      }

      const std::vector<Generic_Parameter> &get_generic_parameters() const {
        return generic_parameters;
      }

      virtual bool has_block() const {
        return false;
      }

      void add_parameter(Parameter_Owner parameter) {
        signature.add_parameter(std::move(parameter));
      }

      void add_parameters(std::vector<Parameter_Owner> &new_parameters) {
        for (auto &parameter: new_parameters) {
          signature.add_parameter(std::move(parameter));
        }
      }

      const std::vector<std::unique_ptr<Parameter>> &get_parameters() const {
        return signature.get_elements();
      }

//      const Profession *get_profession() const override {
//        return signature.get_last().get_profession();
//      }
  };

  using Function_Owner = std::unique_ptr<Function>;

  class Virtual_Function : public Function {

  public:
      Virtual_Function(const std::string &name, const source_mapping::Source_Range &source, Scope &parent) :
        Function(name, source, parent) {}
  };

  class Function_With_Block : public Function {
      Block block;

  public:
      Function_With_Block(const std::string &name, const source_mapping::Source_Range &source,
                          Scope &parent) :
        Function(name, source, parent), block(parent) {}

      Block &get_block() {
        return block;
      }

      const Block &get_block() const {
        return block;
      }

      bool has_block() const override {
        return true;
      }
  };

  using Function_With_Block_Owner = std::unique_ptr<Function_With_Block>;

}