#pragma once

#include <underworld/schema/professions.h>
#include <underworld/schema/Profession_Library.h>
#include "Expression.h"

namespace underworld {

  class Literal : public virtual Expression {
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
  class Literal_Implementation : public Literal {
      T value;

  public:
      Literal_Implementation(T value) : value(value) {}

      const T &get_value() const {
        return value;
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