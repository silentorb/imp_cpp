#pragma once

#include "Enchantment.h"
#include "Minion.h"

namespace overworld {

  class Enchantment_With_Parameters : public Simple_Enchantment {
      std::vector<Simple_Minion_Owner> parameters;

  public:
      Enchantment_With_Parameters(const std::string &name, std::vector<Simple_Minion *> parameters)
        : Simple_Enchantment(name) {
        for (auto &parameter : parameters) {
          this->parameters.push_back(Simple_Minion_Owner(parameter));
        }
      }

      virtual ~Enchantment_With_Parameters() {}

      const std::vector<Simple_Minion_Owner> &get_parameters() const {
        return parameters;
      }
  };

  class Enchantment_With_Arguments : public Enchantment_Reference {
      std::vector<Expression_Owner> arguments;

  public:
      explicit Enchantment_With_Arguments(Enchantment_With_Parameters &parent) :
        Enchantment_Reference(parent) {}

      ~Enchantment_With_Arguments() override = default;

      void add_argument(Expression_Owner argument) {
        arguments.push_back(std::move(argument));
      }

      const std::vector<Expression_Owner> &get_arguments() const {
        return arguments;
      }
  };

}