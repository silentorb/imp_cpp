#pragma once

#include <underworld/schema/Dungeon.h>
#include <underworld/schema/Scope.h>

namespace summoning {

  class Context {
      underworld::Dungeon &dungeon;
      underworld::Scope &scope;

  protected:
      Context(underworld::Dungeon &dungeon, underworld::Scope &scope) :
        dungeon(dungeon), scope(scope) {}

  public:
      underworld::Dungeon &get_dungeon() const {
        return dungeon;
      }
  };

  class Root_Context : public Context {
  public:
      Root_Context(underworld::Dungeon &dungeon) :
        Context(dungeon, dungeon) {}

  };

  class Child_Context : public Context {
      const Context &parent;

  public:
      Child_Context(const Context &parent, underworld::Scope &scope) :
        Context(parent.get_dungeon(), scope), parent(parent) {}

      Child_Context(const Context &parent, underworld::Dungeon &dungeon) :
        Context(dungeon, dungeon), parent(parent) {}
  };
}