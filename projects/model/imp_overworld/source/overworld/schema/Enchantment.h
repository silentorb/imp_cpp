#pragma once

#include <string>
#include <vector>
#include <overworld/expressions/Expression.h>

namespace overworld {

  class Dungeon;

  class Enchantment {
      Dungeon &dungeon;
      std::vector<Expression_Owner> arguments;

  public:
      Enchantment(Dungeon &dungeon) : dungeon(dungeon) {}

      const std::string get_name() const;

      const Dungeon &get_dungeon() const {
        return dungeon;
      }

      const std::string get_argument_string(int index) const;

      void add_argument(Expression_Owner argument) {
        arguments.push_back(std::move(argument));
      }

      const std::vector<Expression_Owner> &get_arguments() const {
        return arguments;
      }

  };

  using Enchantment_Owner = std::unique_ptr<Enchantment>;

//
//  class Simple_Enchantment : public Enchantment {
//      std::string name;
//
//  public:
//      Simple_Enchantment(const std::string &name) : name(name) {}
//
//      ~Simple_Enchantment() override = default;
//
//      const std::string get_name() const override {
//        return name;
//      }
//
//      const Enchantment *get_type() const override {
//        return this;
//      }
//
//      const std::string get_argument_string(int index) const override {
//        return "";
//      }
//  };
//
//  class Enchantment_Reference : public Enchantment {
//  protected:
//      Enchantment &parent;
//
//  public:
//      explicit Enchantment_Reference(Enchantment &parent) : parent(parent) {}
//
//      ~Enchantment_Reference() override = default;
//
//      const Enchantment *get_type() const override {
//        return parent.get_type();
//      }
//
//      const std::string get_name() const override {
//        return parent.get_name();
//      }
//
//      const std::string get_argument_string(int index) const override {
//        return "";
//      }
//  };


}