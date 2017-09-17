#pragma once

#include <string>
#include <memory>
#include <vector>
#include "underworld/expressions/Expression.h"

namespace underworld {

  class Enchantment {
      std::string name;
      std::vector<Expression_Owner> arguments;

  public:
      Enchantment(const std::string &name) : name(name) {}

      const std::string &get_name() const {
        return name;
      }

      void add_argument(Expression_Owner argument) {
        arguments.push_back(std::move((argument)));
      }

      const std::vector<Expression_Owner> &get_arguments() const {
        return arguments;
      }
  };

  using Enchantment_Owner = std::unique_ptr<Enchantment>;
  using Enchantment_Array = std::vector<Enchantment_Owner>;
}