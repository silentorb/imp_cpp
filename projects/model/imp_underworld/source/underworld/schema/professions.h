#pragma once

#include <memory>
#include "Profession.h"

namespace underworld {

  class Dungeon;

  class Unknown : public virtual Profession {
  public:
      Type get_type() const override {
        return Type::unknown;
      }

      const std::string get_name() const override {
        return "unknown";
      }
  };

  enum class Primitive_Type {
      Bool,
      Double,
      Float,
      Int,
      String,
      Void,
  };

  const int Primitive_Type_Count = 6;

  class Primitive : public virtual Profession {
      Primitive_Type primitive_type;
      const std::string name;

  public:
      Primitive(Primitive_Type primitive_type, const std::string &name) :
        primitive_type(primitive_type), name(name) {}

      Type get_type() const override {
        return Type::primitive;
      }

      Primitive_Type get_primitive_type() const {
        return primitive_type;
      }

      const std::string get_name() const override {
        return name;
      }
  };

//  class Professional_Dungeon : public virtual Profession {
//      Dungeon &dungeon;
//
//  public:
//      Professional_Dungeon(Dungeon &dungeon) :
//        dungeon(dungeon) {}
//
//      Type get_type() const override {
//        return Type::dungeon;
//      }
//  };

  class Reference : public virtual Profession {
      Profession &profession;

  public:
      Reference(Profession &profession) :
        profession(profession) {}

      Type get_type() const override {
        return Type::reference;
      }
  };

  class Function_Signature : public virtual Profession {
  public:
      Type get_type() const override {
        return Type::function;
      }
  };

}