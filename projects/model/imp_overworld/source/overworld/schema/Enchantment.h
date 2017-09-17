#pragma once

#include <string>
#include <vector>
#include <overworld/expressions/Expression.h>

namespace overworld {

  class Enchantment {

  public:
      virtual const std::string get_name() const = 0;
      virtual bool matches(const Enchantment &other) const = 0;
      virtual ~Enchantment() = default;
  };

  class Simple_Enchantment : public Enchantment {
      std::string name;

  public:
      Simple_Enchantment(const std::string &name) : name(name) {}

      ~Simple_Enchantment() override = default;

      const std::string get_name() const override {
        return name;
      }

      bool matches(const Enchantment &other) const override {
        return &other == this;
      }
  };

  class Enchantment_Reference : public Enchantment {
  protected:
      Enchantment &parent;

  public:
      explicit Enchantment_Reference(Enchantment &parent) : parent(parent) {}

      ~Enchantment_Reference() override = default;

      bool matches(const Enchantment &other) const override {
        return &other == this || parent.matches(other);
      }

      const std::string get_name() const override {
        return parent.get_name();
      }
  };

  class Enchantment_With_Arguments : public Enchantment_Reference {
      std::vector<Expression_Owner> arguments;

  public:
      explicit Enchantment_With_Arguments(Enchantment &parent) : Enchantment_Reference(parent) {}

      ~Enchantment_With_Arguments() override = default;

      void add_argument(Expression_Owner argument) {
        arguments.push_back(std::move(argument));
      }

  };

  using Enchantment_Owner = std::unique_ptr<Enchantment>;

  class Enchantment_Container {
      std::vector<Enchantment_Owner> enchantments;

  public:
      Enchantment_Container() = default;

      Enchantment_Container(const Enchantment_Container&) = delete;

      void add_enchantment(Enchantment_Owner enchantment) {
        if (!has_enchantment(*enchantment))
          enchantments.push_back(std::move(enchantment));
      }

      void add_enchantment(Enchantment &enchantment) {
        if (!has_enchantment(enchantment))
          enchantments.push_back(Enchantment_Owner(new Enchantment_Reference(enchantment)));
      }

      Enchantment *get_enchantment(const Enchantment &enchantment) const {
        for (auto &entry : enchantments) {
          if (entry->matches(enchantment))
            return entry.get();
        }
        return nullptr;
      }

      bool has_enchantment(const Enchantment &enchantment) const {
        return get_enchantment(enchantment) != nullptr;
      }
  };
}