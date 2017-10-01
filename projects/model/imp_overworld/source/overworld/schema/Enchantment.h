#pragma once

#include <string>
#include <vector>
#include <overworld/expressions/Expression.h>

namespace overworld {

  class Enchantment {

  public:
      virtual const std::string get_name() const = 0;
      virtual const Enchantment *get_type() const = 0;
      virtual ~Enchantment() = default;
      virtual const std::string get_argument_string(int index) const = 0;
  };

  class Simple_Enchantment : public Enchantment {
      std::string name;

  public:
      Simple_Enchantment(const std::string &name) : name(name) {}

      ~Simple_Enchantment() override = default;

      const std::string get_name() const override {
        return name;
      }

      const Enchantment *get_type() const override {
        return this;
      }

      const std::string get_argument_string(int index) const override {
        return "";
      }
  };

  class Enchantment_Reference : public Enchantment {
  protected:
      Enchantment &parent;

  public:
      explicit Enchantment_Reference(Enchantment &parent) : parent(parent) {}

      ~Enchantment_Reference() override = default;

      const Enchantment *get_type() const override {
        return parent.get_type();
      }

      const std::string get_name() const override {
        return parent.get_name();
      }

      const std::string get_argument_string(int index) const override {
        return "";
      }
  };

  using Enchantment_Owner = std::unique_ptr<Enchantment>;

}