#pragma once

#include <memory>
#include <underworld/Source_Point.h>
#include <overworld/schema/Dungeon.h>
#include "Profession.h"

namespace overworld {

  class Unknown : public virtual Profession {
  public:
      Profession_Type get_type() const override {
        return Profession_Type::unknown;
      }

      Scope *get_scope() override {
        return nullptr;
      }

      File *get_file() const override {
        return nullptr;
      }

      const std::string get_name() const override {
        return "?";
      }

      Ownership get_ownership() const override {
        return Ownership::value;
      }

      Profession &get_base() override {
        return *this;
      }

      const Profession &get_base() const override {
        return *this;
      }
  };

  class Void : public virtual Profession {
  public:
      Profession_Type get_type() const override {
        return Profession_Type::Void;
      }

      Scope *get_scope() override {
        return nullptr;
      }

      File *get_file() const override {
        return nullptr;
      }

      const std::string get_name() const override {
        return "void";
      }

      Ownership get_ownership() const override {
        return Ownership::value;
      }

      Profession &get_base() override {
        return *this;
      }

      const Profession &get_base() const override {
        return *this;
      }
  };

  enum class Primitive_Type {
      Bool,
      Double,
      Float,
      Int,
      String,
//      Void
  };

  const int Primitive_Type_Count = 5;

  class Primitive : public virtual Profession {
      Primitive_Type primitive_type;
      File *file = nullptr;

  public:
      Primitive(Primitive_Type primitive_type, File *file = nullptr) :
        primitive_type(primitive_type), file(file) {}

      virtual ~Primitive() {}

      Profession_Type get_type() const override {
        return Profession_Type::primitive;
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

      const std::string get_name() const override;

      Ownership get_ownership() const override {
        return Ownership::value;
      }

      Profession &get_base() override {
        return *this;
      }

      const Profession &get_base() const override {
        return *this;
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

      virtual ~Reference() {}

      Profession_Type get_type() const override {
        return Profession_Type::reference;
      }

      Scope *get_scope() override {
        return nullptr;
      }

      File *get_file() const override {
        return nullptr;
      }

      Ownership get_ownership() const override {
        return Ownership::reference;
      }

      const std::string get_name() const override {
        return "&" + profession.get_name();
      }

      Profession &get_base() override {
        return profession;
      }

      const Profession &get_base() const override {
        return profession;
      }
  };

//  class Function_Signature : public virtual Profession {
//  public:
//      Type get_type() const override {
//        return Type::function;
//      }
//
//      Scope *get_scope() override {
//        return nullptr;
//      }
//
//      File *get_file() const override {
//        return nullptr;
//      }
//  };

}