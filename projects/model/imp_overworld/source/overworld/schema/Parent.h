#pragma once

namespace overworld {

  class Dungeon_Interface;

  class Function_Interface;

  enum class Parent_Type {
      none,
      dungeon,
      function,
  };

  class Parent {
      Parent_Type type;
      union {
          Dungeon_Interface *dungeon;
          Function_Interface *function;
      };

  public:
      Parent() :
        dungeon(nullptr), type(Parent_Type::none) {}

      Parent(Dungeon_Interface &dungeon) :
        dungeon(&dungeon), type(Parent_Type::dungeon) {}

      Parent(Function_Interface &function) :
        function(&function), type(Parent_Type::function) {}

      Parent_Type get_type() const {
        return type;
      }

      Dungeon_Interface &get_dungeon() {
        if (type != Parent_Type::dungeon)
          throw std::runtime_error("Parent is not a dungeon.");

        return *dungeon;
      }

      Function_Interface &get_function() {
        if (type != Parent_Type::function)
          throw std::runtime_error("Parent is not a function.");

        return *function;
      }
  };
}