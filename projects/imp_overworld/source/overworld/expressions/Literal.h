#pragma once

#include <overworld/schema/professions.h>
#include <overworld/schema/Profession_Library.h>
#include "Expression.h"

namespace overworld {

  template<typename T>
  class Literal : public virtual Expression {
      T value;

  public:
      Literal(T value) : value(value) {}

      Type get_type() override {
        return Type::literal;
      }

      virtual Profession &get_profession() = 0;
  };

  class Literal_Int : public Literal<int> {
  public:
      Literal_Int(int value) : Literal(value) {}

      Profession &get_profession() override {
        return Profession_Library::get_primitive(Primitive_Type::Int);
      }
  };

  class Literal_String : public Literal<const std::string> {
  public:
      Literal_String(const std::string &value) : Literal(value) {}

      Profession &get_profession() override {
        return Profession_Library::get_primitive(Primitive_Type::String);
      }
  };


  class Literal_Bool : public Literal<bool> {
  public:
      Literal_Bool(const bool &value) : Literal(value) {}

      Profession &get_profession() override {
        return Profession_Library::get_primitive(Primitive_Type::Bool);
      }
  };
}