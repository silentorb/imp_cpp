#pragma once

#include <underworld/schema/Dungeon.h>
#include <underworld/schema/Scope.h>

namespace imp_summoning {

  class Context {
  protected:
      underworld::Scope &scope;
      underworld::Enchantment_Array enchantments;

      Context(underworld::Scope &scope) :
        scope(scope) {}

      underworld::Member *find_member_internal(const std::string &name) const;

  public:
//      underworld::Dungeon &get_dungeon() const {
//        return dungeon;
//      }

      underworld::Scope &get_scope() const {
        return scope;
      }

      underworld::Scope &get_scope() {
        return scope;
      }

      virtual underworld::Member *find_member(const std::string &name) const = 0;

      void add_enchantment(underworld::Enchantment_Owner enchantment) {
        enchantments.push_back(std::move(enchantment));
      }

      underworld::Enchantment_Array &get_enchantments() {
        return enchantments;
      }
  };

  class Root_Context : public Context {
  public:
      Root_Context(underworld::Dungeon &dungeon) :
        Context(dungeon.get_scope()) {}

      underworld::Member *find_member(const std::string &name) const override {
        return find_member_internal(name);
      }
  };

  class Child_Context : public Context {
      const Context &parent;

  public:
      Child_Context(const Context &parent, underworld::Scope &scope) :
        Context(scope), parent(parent) {}

      Child_Context(const Context &parent, underworld::Dungeon &dungeon) :
        Context(dungeon.get_scope()), parent(parent) {}

      underworld::Member *find_member(const std::string &name) const override;
  };
}