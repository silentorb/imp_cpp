#pragma once

#include <underworld/schema/Dungeon.h>

namespace cpp_scanning {

  enum class Access_Mode {
      none,
      Private,
      Protected,
      Public,
  };

  class Context {
      underworld::Dungeon &dungeon;
      Access_Mode access_mode;

  public:
      Context(underworld::Dungeon &dungeon, Access_Mode access_mode = Access_Mode::none) :
        dungeon(dungeon), access_mode(access_mode) {}

      underworld::Dungeon &get_dungeon() const {
        return dungeon;
      }

      Access_Mode get_access_mode() const {
        return access_mode;
      }

      void set_access_mode(Access_Mode access_mode) {
        Context::access_mode = access_mode;
      }

      bool is_class() const {
        return access_mode != Access_Mode::none;
      }
  };
}