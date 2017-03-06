#pragma once

namespace overworld {

  class Member {
  public:
      enum class Type {
          minion,
          portal
      };

      virtual Type get_type() const = 0;
  };

  using Member_Pointer = std::unique_ptr<Member>;
}