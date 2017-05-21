#pragma once

#include <overworld/schema/professions.h>
#include <overworld/schema/Profession_Library.h>
#include "Expression.h"

namespace overworld {

  class Literal : public Expression {

  public:
      Type get_type() const override {
        return Type::literal;
      }

      virtual Primitive_Type get_primitive_type() const = 0;

      Profession &get_profession() const {
        return Profession_Library::get_primitive(get_primitive_type());
      }
  };

  template<typename T>
  class Literal_Implementation : public virtual Literal {
      T value;
      Profession_Node<Literal_Implementation> node;

  public:
      Literal_Implementation(T value) : value(value), node(*this) {}

      const T &get_value() const {
        return value;
      }

      Node *get_node() override {
        return & node;
      }
  };

  class Literal_Int : public Literal_Implementation<int> {
  public:
      Literal_Int(int value) : Literal_Implementation(value) {}

      Primitive_Type get_primitive_type() const override {
        return Primitive_Type::Int;
      }
  };

  class Literal_String : public Literal_Implementation<const std::string> {
  public:
      Literal_String(const std::string &value) : Literal_Implementation(value) {}

      Primitive_Type get_primitive_type() const override {
        return Primitive_Type::String;
      }
  };

  class Literal_Bool : public Literal_Implementation<bool> {
  public:
      Literal_Bool(const bool &value) : Literal_Implementation(value) {}

      Primitive_Type get_primitive_type() const override {
        return Primitive_Type::Bool;
      }
  };
}