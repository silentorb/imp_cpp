#pragma once

#include <overworld/schema/professions/professions.h>
#include <overworld/schema/professions/Profession_Library.h>
#include "Expression.h"

namespace overworld {

  class Literal : public Common_Node_Expression {

  public:
      Literal(Parent parent, const source_mapping::Source_Range &source_range) :
        Common_Node_Expression(parent, source_range) {}

      Expression_Type get_type() const override {
        return Expression_Type::literal;
      }

      virtual Primitive_Type get_primitive_type() const = 0;

      Profession_Reference &get_profession() override {
        return Profession_Library::get_primitive(get_primitive_type());
      }

      const Profession_Reference &get_profession() const override {
        return Profession_Library::get_primitive(get_primitive_type());
      }
  };

  template<typename T, typename Static_Functions>
  class Literal_Implementation : public Literal {
      T value;
      Node node;

  public:
      Literal_Implementation(T value, Parent parent, const source_mapping::Source_Range &source_range) :
        Literal(parent, source_range),
        value(value),
        node(Profession_Library::get_primitive(Static_Functions::_get_primitive_type()), element) {
//        node.set_status(Node_Status::resolved);
      }

      Primitive_Type get_primitive_type() const override {
        return Static_Functions::_get_primitive_type();
      }

      virtual ~Literal_Implementation() override {}

      const T &get_value() const {
        return value;
      }

      const Node *get_node() const override {
        return &node;
      }

      Node *get_node() override {
        return &node;
      }

//      void set_profession(Profession_Reference &value) override {
//        throw std::runtime_error("Not supported.");
//      }
  };

  class Literal_Int : public Literal_Implementation<int, Literal_Int> {

  public:
      Literal_Int(int value, Parent parent, const source_mapping::Source_Range &source_point) :
        Literal_Implementation(value, parent, source_point) {}

      static Primitive_Type _get_primitive_type() {
        return Primitive_Type::Int;
      }

      const std::string get_name() const override {
        return "int";
      }
  };

  class Literal_String : public Literal_Implementation<const std::string, Literal_String> {
  public:
      Literal_String(const std::string &value, Parent parent, const source_mapping::Source_Range &source_point) :
        Literal_Implementation(value, parent, source_point) {}

      static Primitive_Type _get_primitive_type() {
        return Primitive_Type::String;
      }

      const std::string get_name() const override {
        return "string";
      }
  };

  class Literal_Bool : public Literal_Implementation<bool, Literal_Bool> {
  public:
      Literal_Bool(const bool &value, Parent parent, const source_mapping::Source_Range &source_point) :
        Literal_Implementation(value, parent, source_point) {}

      static Primitive_Type _get_primitive_type() {
        return Primitive_Type::Bool;
      }

      const std::string get_name() const override {
        return "bool";
      }
  };
}