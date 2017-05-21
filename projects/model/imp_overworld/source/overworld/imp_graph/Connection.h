#pragma once

#include <memory>
#include <graphing/Connection.h>

namespace overworld {

  class Node;

  class Connection : public graphing::Connection<Node> {
      bool conflicts = false;

  public:
      Connection(Node &first, Node &second) : graphing::Connection<Node>(first, second) {}

      bool does_conflict() const {
        return conflicts;
      }

      void set_conflicts(bool conflicts) {
        Connection::conflicts = conflicts;
      }
  };

  using Connection_Pointer = std::unique_ptr<Connection>;

}