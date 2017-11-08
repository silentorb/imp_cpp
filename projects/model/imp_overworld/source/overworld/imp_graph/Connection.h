#pragma once

#include <memory>
#include <graphing/Generic_Connection.h>
#include "Component_Selection.h"

namespace overworld {

  class Node;

  class Profession_Reference;

  enum class Connection_Type {
      direct_assignment,
      direct_general,

      compound_to_scalar,
      container_to_member,
      lambda_to_parameter,
      variant_to_lambda,
  };

  class Connection : public graphing::Generic_Connection<Node> {
      Connection_Type type;

  public:
      Connection(Node &first, Node &second, Connection_Type type = Connection_Type::direct_general) :
        graphing::Generic_Connection<Node>(first, second), type(type) {}

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

      bool is_direct() const {
        return type == Connection_Type::direct_general
               || type == Connection_Type::direct_assignment;
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

  class Variant_To_Lambda : public Connection {
      int variant_parameter_index;
      int lambda_parameter_index;

  public:
      Variant_To_Lambda(Node &first, Node &second, int variant_parameter_index, int lambda_parameter_index) :
        Connection(first, second, Connection_Type::variant_to_lambda),
        variant_parameter_index(variant_parameter_index),
        lambda_parameter_index(lambda_parameter_index) {}

      Profession_Reference get_profession(Node &node) override;
  };

  class Lambda_To_Parameter : public Connection {
      int lambda_parameter_index;

  public:
      Lambda_To_Parameter(Node &first, Node &second, int lambda_parameter_index) :
        Connection(first, second, Connection_Type::lambda_to_parameter),
        lambda_parameter_index(lambda_parameter_index) {}

      Profession_Reference get_profession(Node &node) override;
  };

//  class Container_To_Member : public Connection {
//      std::string member_name;
//
//  public:
//      Container_To_Member(Node &first, Node &second, const std::string &member_name) :
//        Connection(first, second, Connection_Type::lambda_to_parameter),
//        member_name(member_name) {}
//
//      Profession_Reference get_profession(Node &node) override;
//  };
}