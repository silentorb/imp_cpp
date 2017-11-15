#pragma once

#include <memory>
#include "Profession.h"

namespace underworld {

  enum class Primitive_Type {
      Bool,
      Double,
      Float,
      Int,
      String,
      Unknown
  };

  const int Primitive_Type_Count = 6;

  class Primitive : public Profession {
      Primitive_Type primitive_type;
      const source_mapping::Source_Range source_point;

  public:
      Primitive(Primitive_Type primitive_type, const source_mapping::Source_Range &source_point) :
        primitive_type(primitive_type), source_point(source_point) {}

      Profession_Type get_type() const override {
        return Profession_Type::primitive;
      }

      Primitive_Type get_primitive_type() const {
        return primitive_type;
      }

      const std::string get_name() const override {
        return "primitive";
      }

      const source_mapping::Source_Range get_source_point() const override {
        return source_point;
      }
  };

  enum class Decorator_Type {
      owner,
      reference,
      pointer,
  };

  class Decorator : public Profession {
      Profession_Owner profession;
      Decorator_Type type;

  public:
      Decorator(Decorator_Type type, Profession_Owner profession) :
        profession(std::move(profession)), type(type) {}

      Profession_Type get_type() const override {
        return Profession_Type::decorator;
      }

      const std::string get_name() const override {
        return profession->get_name();
      }

      const source_mapping::Source_Range get_source_point() const override {
        return profession->get_source_point();
      }

      const Profession &get_profession() const {
        return *profession;
      }

      Decorator_Type get_decorator_type() const {
        return type;
      }
  };

//  class Pointer : public Decorator {
//  public:
//      Pointer(Profession_Owner profession) : Decorator(std::move(profession)) {}
//
//      Profession_Type get_type() const override {
//        return Profession_Type::pointer;
//      }
//  };

//  class Function_Signature : public virtual Profession {
//  public:
//      Type get_type() const override {
//        return Type::function;
//      }
//  };
//
}