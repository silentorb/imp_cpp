#pragma once

#include <memory>
#include <graphing/Connection.h>
#include "Component_Selection.h"

namespace overworld {

  class Node;

  class Profession_Reference;

  enum class Connection_Type {
      compound_to_scalar,
      direct,
      lambda,
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
        int k = 0;
      }

      Connection(const Connection &) = delete;

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

      virtual Profession_Reference get_profession(Node &node);
  };

  using Connection_Pointer = std::unique_ptr<Connection>;

  class Variant_To_Argument : public Connection {
      int parameter_index;

  public:
      Variant_To_Argument(Node &first, Node &second, int parameter_index) :
        Connection(first, second, Connection_Type::compound_to_scalar),
        parameter_index(parameter_index) {}

      virtual ~Variant_To_Argument() {
        int k = 0;
      }

      Profession_Reference get_profession(Node &node) override;


  };

  class Lambda_To : public Connection {

  public:
      Lambda_To(Node &first, Node &second) :
        Connection(first, second, Connection_Type::lambda) {}

      Profession_Reference get_profession(Node &node) override;
  };
}