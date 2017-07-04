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
      Void,
      Unknown,
  };

  const int Primitive_Type_Count = 6;

  class Primitive : public virtual Profession {
      Primitive_Type primitive_type;

  public:
      Primitive(Primitive_Type primitive_type) :
        primitive_type(primitive_type) {}

      Type get_type() const override {
        return Type::primitive;
      }

      Primitive_Type get_primitive_type() const {
        return primitive_type;
      }

      const std::string get_name() const override {
        return "primitive";
      }
  };

  class Reference : public virtual Profession {
      Profession &profession;

  public:
      Reference(Profession &profession) :
        profession(profession) {}

      Type get_type() const override {
        return Type::reference;
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