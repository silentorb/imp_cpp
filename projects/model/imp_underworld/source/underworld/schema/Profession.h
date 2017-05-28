#pragma once

#include <string>

namespace underworld {

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
      virtual const std::string get_name() const = 0;
  };
}