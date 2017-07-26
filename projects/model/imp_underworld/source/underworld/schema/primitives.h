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
      Unknown,
  };

  const int Primitive_Type_Count = 6;

  class Primitive : public virtual Profession {
      Primitive_Type primitive_type;
      const Source_Point source_point;

  public:
      Primitive(Primitive_Type primitive_type, const Source_Point &source_point) :
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

      const Source_Point get_source_point() const override {
        return source_point;
      }
  };

  class Reference : public virtual Profession {
      Profession &profession;

  public:
      Reference(Profession &profession) :
        profession(profession) {}

      Profession_Type get_type() const override {
        return Profession_Type::reference;
      }
  };

//  class Function_Signature : public virtual Profession {
//  public:
//      Type get_type() const override {
//        return Type::function;
//      }
//  };
//
}