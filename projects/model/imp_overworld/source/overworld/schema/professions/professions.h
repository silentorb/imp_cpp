#pragma once

#include <memory>
#include <underworld/Source_Point.h>
#include <overworld/schema/Dungeon.h>
#include "Profession.h"

namespace overworld {

  class Unknown : public virtual Profession {
  public:
      Type get_type() const override {
        return Type::unknown;
      }

      Scope *get_scope() override {
        return nullptr;
      }

      File *get_file() const override {
        return nullptr;
      }
  };

  enum class Primitive_Type {
      Bool,
      Double,
      Float,
      Int,
      String,
      Void
  };

  const int Primitive_Type_Count = 6;

  class Primitive : public virtual Profession {
      Primitive_Type primitive_type;
      File *file = nullptr;

  public:
      Primitive(Primitive_Type primitive_type, File *file = nullptr) :
        primitive_type(primitive_type), file(file) {}

      Type get_type() const override {
        return Type::primitive;
      }

      Primitive_Type get_primitive_type() const {
        return primitive_type;
      }

      Scope *get_scope() override {
        return nullptr;
      }

      File *get_file() const override {
        return file;
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
//
//      Dungeon *get_parent_dungeon() override {
//        return dungeon.get_parent();
//      }
//
//      File *get_file() override {
//        return nullptr;
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

      Scope *get_scope() override {
        return nullptr;
      }

      File *get_file() const override {
        return nullptr;
      }
  };

  class Function_Signature : public virtual Profession {
  public:
      Type get_type() const override {
        return Type::function;
      }

      Scope *get_scope() override {
        return nullptr;
      }

      File *get_file() const override {
        return nullptr;
      }
  };

}