#pragma once

#include <stdexcept>

namespace overworld {

  class Dungeon;

  class Function;

  enum class Parent_Type {
      none,
      dungeon,
      function,
  };

  class Parent {
      Parent_Type type;
      union {
          Dungeon *dungeon;
          Function *function;
      };

  public:
      Parent() :
        dungeon(nullptr), type(Parent_Type::none) {}

      Parent(Dungeon &dungeon) :
        dungeon(&dungeon), type(Parent_Type::dungeon) {}

      Parent(Function &function) :
        function(&function), type(Parent_Type::function) {}

      Parent_Type get_type() const {
        return type;
      }

      Parent *get_parent() const;

      const std::string get_name() const;

      Dungeon &get_dungeon() {
        if (type != Parent_Type::dungeon)
          throw std::runtime_error("Parent is not a dungeon.");

        return *dungeon;
      }

      const Dungeon &get_dungeon() const {
        if (type != Parent_Type::dungeon)
          throw std::runtime_error("Parent is not a dungeon.");

        return *dungeon;
      }

      Function &get_function() {
        if (type != Parent_Type::function)
          throw std::runtime_error("Parent is not a function.");

        return *function;
      }

      bool is_descendant_of(const Parent &other) const;
  };

  const std::string get_namespace_string(const Parent &parent, const std::string &delimiter);

}