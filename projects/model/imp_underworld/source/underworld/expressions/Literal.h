#pragma once

#include <underworld/schema/primitives.h>
#include <source_mapping/Source_Point.h>
#include "Expression.h"

namespace underworld {

  class Literal : public virtual Common_Expression {
      source_mapping::Source_Point source_point;

  public:
      Literal(const source_mapping::Source_Point &source_point) : source_point(source_point) {}

      virtual ~Literal() {

      }

      Type get_type() const override {
        return Type::literal;
      }

      virtual Primitive_Type get_primitive_type() const = 0;


      const source_mapping::Source_Point get_source_point() const override {
        return source_point;
      }
  };

  template<typename T>
  class Literal_Implementation : public Literal {
      T value;

  public:
      Literal_Implementation(T value, const source_mapping::Source_Point &source) :
        value(value), Literal(source) {}

      virtual ~Literal_Implementation() {

      }

      const T &get_value() const {
        return value;
      }
  };

  class Literal_Int : public Literal_Implementation<int> {
  public:
      Literal_Int(int value, const source_mapping::Source_Point &source) :
        Literal_Implementation(value, source) {}

      Primitive_Type get_primitive_type() const override {
        return Primitive_Type::Int;
      }
  };

  class Literal_String : public Literal_Implementation<const std::string> {
  public:
      Literal_String(const std::string &value, const source_mapping::Source_Point &source) :
        Literal_Implementation(value, source) {}

      Primitive_Type get_primitive_type() const override {
        return Primitive_Type::String;
      }
  };

  class Literal_Bool : public Literal_Implementation<bool> {
  public:
      Literal_Bool(const bool &value, const source_mapping::Source_Point &source) :
        Literal_Implementation(value, source) {}

      Primitive_Type get_primitive_type() const override {
        return Primitive_Type::Bool;
      }
  };
}