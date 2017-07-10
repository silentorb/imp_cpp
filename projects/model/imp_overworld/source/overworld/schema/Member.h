#pragma once

#include <overworld/imp_graph/Node.h>

namespace overworld {

  enum class Member_Type {
      dungeon,
      function,
      variable,
      unresolved,
  };

  class Member {
  public:

      virtual Member_Type get_member_type() const = 0;
      virtual Node &get_node() = 0;
      virtual const std::string get_name() const = 0;

      virtual ~Member() {}
  };

  using Member_Pointer = std::unique_ptr<Member>;
}