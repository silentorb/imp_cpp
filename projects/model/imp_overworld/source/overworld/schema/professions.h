#pragma once

#include <memory>

namespace overworld {

  class Dungeon;

  class Profession {
  public:

      enum class Type {
          unknown,
          primitive,
          dungeon,
          reference,
          function,
      };

      virtual Type get_type() const = 0;

  };

  class Unknown : public virtual Profession {
  public:
      Type get_type() const override {
        return Type::unknown;
      }
  };

  enum class Primitive_Type {
      Bool,
      Double,
      Float,
      Int,
      String,
  };

  const int Primitive_Type_Count = 5;

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
  };

  class Professional_Dungeon : public virtual Profession {
      Dungeon &dungeon;

  public:
      Professional_Dungeon(Dungeon &dungeon) :
        dungeon(dungeon) {}

      Type get_type() const override {
        return Type::dungeon;
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

  class Function_Signature : public virtual Profession {
  public:
      Type get_type() const override {
        return Type::function;
      }
  };

}