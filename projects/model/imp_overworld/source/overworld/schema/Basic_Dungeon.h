#pragma once

#include <overworld/schema/professions/Profession.h>

namespace overworld {

  enum class Dungeon_Type {
      original,
      variant
  };

  class Basic_Dungeon {
  public:
      virtual ~Basic_Dungeon() = default;
      virtual Dungeon &get_original() = 0;
      virtual const Dungeon &get_original() const = 0;
      virtual Dungeon_Type get_dungeon_type() const = 0;
      virtual Ownership get_ownership() const = 0;
      virtual const std::string get_debug_name() const = 0;
  };

  using Dungeon_Interface_Owner = std::unique_ptr<Basic_Dungeon>;
};