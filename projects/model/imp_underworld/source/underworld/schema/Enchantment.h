#pragma once

#include <string>
#include <memory>
#include <vector>
#include "underworld/expressions/Expression.h"

namespace underworld {

  class Enchantment {
      Profession_Owner profession;
      std::vector<Expression_Owner> arguments;

  public:
      Enchantment(Profession_Owner profession) : profession(std::move(profession)) {}

//      const std::string &get_name() const {
//        return name;
//      }

      const Profession &get_profession() const {
        return *profession;
      }

      void add_argument(Expression_Owner argument) {
        arguments.push_back(std::move((argument)));
      }

      const std::vector<Expression_Owner> &get_arguments() const {
        return arguments;
      }
  };

  using Enchantment_Owner = std::shared_ptr<Enchantment>;
  using Enchantment_Array = std::vector<Enchantment_Owner>;
}