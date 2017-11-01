#pragma once

namespace overworld {

  class Basic_Dungeon;

  class Function_Interface;

  enum class Parent_Type {
      none,
      dungeon,
      function,
  };

  class Parent {
      Parent_Type type;
      union {
          Basic_Dungeon *dungeon;
          Function_Interface *function;
      };

  public:
      Parent() :
        dungeon(nullptr), type(Parent_Type::none) {}

      Parent(Basic_Dungeon &dungeon) :
        dungeon(&dungeon), type(Parent_Type::dungeon) {}

      Parent(Function_Interface &function) :
        function(&function), type(Parent_Type::function) {}

      Parent_Type get_type() const {
        return type;
      }

      const std::string get_name() const;

      Basic_Dungeon &get_dungeon() {
        if (type != Parent_Type::dungeon)
          throw std::runtime_error("Parent is not a dungeon.");

        return *dungeon;
      }

     const Basic_Dungeon &get_dungeon() const{
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