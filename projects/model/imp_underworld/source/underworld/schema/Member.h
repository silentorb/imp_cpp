#pragma once

namespace underworld {

  class Member {
  public:
      enum class Type {
          function,
          variable,
      };

      virtual Type get_type() const = 0;
  };

  using Member_Pointer = std::unique_ptr<Member>;
}