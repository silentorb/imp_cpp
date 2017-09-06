#pragma once

#include <memory>
#include <graphing/Connection.h>

namespace overworld {

  class Node;

  enum class Connection_Type {
      member,
      normal,
  };

  class Connection : public graphing::Connection<Node> {
      bool conflicts = false;
      Connection_Type type = Connection_Type::normal;

  public:
      Connection(Node &first, Node &second) :
        graphing::Connection<Node>(first, second) {}

      virtual ~Connection() {

      }

      bool does_conflict() const {
        return conflicts;
      }

      void set_conflicts(bool conflicts) {
        Connection::conflicts = conflicts;
      }

      Connection_Type get_type() const {
        return type;
      }

      void set_type(Connection_Type value) {
        type = value;
      }
  };

  using Connection_Pointer = std::unique_ptr<Connection>;

}