#pragma once

#include <string>
#include <memory>

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

  using Profession_Owner = std::unique_ptr<Profession>;
}