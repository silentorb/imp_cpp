#pragma once

#include <overworld/expressions/Member_Expression.h>
#include "Minion.h"
#include "Function.h"

namespace overworld {

  class Temporary_Minion : public Minion {
      std::vector<Member_Expression *> expressions;
      std::unique_ptr<Function_Signature> signature;
      std::vector<std::unique_ptr<Parameter>> parameters;

  public:
      Temporary_Minion(const std::string &name, Profession &profession,
                       const source_mapping::Source_Point source_point, Function_Interface &function) :
        Minion(name, profession, nullptr, source_point, &function) {}

      virtual ~Temporary_Minion() {}

      void add_expression(Member_Expression &expression) {
        expressions.push_back(&expression);
      }

      void replace_with(Member &member) {
        for (auto expression: expressions) {
          expression->set_member(member);
        }
      }

      Function_Signature &get_or_create_signature() {
        if (!signature)
          signature = std::unique_ptr<Function_Signature>(new Function_Signature(get_node()));

        return *signature;
      }

      void add_parameter(Parameter *parameter) {
        parameters.push_back(std::unique_ptr<Parameter>(parameter));
        signature->add_parameter(*parameter);
      }
  };
}