#pragma once

#include <overworld/imp_graph/Node.h>

namespace overworld {

  class Member {
  public:
      enum class Type {
          function,
          variable,
          unresolved,
      };

      virtual Type get_type() const = 0;
      virtual Node &get_node() = 0;
      virtual const std::string get_name() const = 0;

      virtual ~Member() {}
  };

  using Member_Pointer = std::unique_ptr<Member>;
}