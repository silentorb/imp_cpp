#pragma once

namespace overworld {

  class Dungeon;

  class File;

  class Profession {
  public:

      enum class Type {
          unknown,
          primitive,
          dungeon,
          reference,
          function,
      };

      virtual Type get_type() const = 0;
      virtual Dungeon *get_parent_dungeon() = 0;
      virtual File *get_file() const = 0;
  };
}