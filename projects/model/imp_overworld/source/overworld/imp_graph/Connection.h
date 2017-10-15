#pragma once

#include <memory>
#include <graphing/Connection.h>
#include "Component_Selection.h"

namespace overworld {

  class Node;

  enum class Connection_Type {
      compound_to_scalar,
      direct,
  };

  class Connection : public graphing::Connection<Node> {
//      bool conflicts = false;
      Connection_Type type = Connection_Type::direct;

  protected:
      Connection(Node &first, Node &second, Connection_Type type) :
        graphing::Connection<Node>(first, second), type(type) {}

  public:
      Connection(Node &first, Node &second) :
        graphing::Connection<Node>(first, second) {}

      virtual ~Connection() {

      }

//      bool does_conflict() const {
//        return conflicts;
//      }
//
//      void set_conflicts(bool conflicts) {
//        Connection::conflicts = conflicts;
//      }

      Connection_Type get_type() const {
        return type;
      }

//      void set_type(Connection_Type value) {
//        type = value;
//      }
  };

  using Connection_Pointer = std::unique_ptr<Connection>;

  class Component_To_Scalar : public Connection {
      Component_Selection_Owner selector;

  public:
      Component_To_Scalar(Node &first, Node &second, Component_Selection_Owner &selector) :
        Connection(first, second, Connection_Type::compound_to_scalar),
        selector(std::move(selector)) {}

      virtual ~Component_To_Scalar() {}
  };
}