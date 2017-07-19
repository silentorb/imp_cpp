#pragma once

namespace overworld {

  class Dungeon;

  class File;

  class Scope;

  class Profession {
  public:

      enum class Type {
          unknown,
          primitive,
          dungeon,
          reference,
          function,
      };

      virtual ~Profession() {}

      virtual Type get_type() const = 0;
      virtual Scope *get_scope() = 0;
      virtual File *get_file() const = 0;
      virtual const std::string get_name() const = 0;

      virtual Node &get_node() {
        throw std::runtime_error("Not implemented.");
      }
  };
}