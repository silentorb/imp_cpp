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

  class Reference : public virtual Profession {
  protected:
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
        return profession.get_name();
      }

      const std::string get_debug_name() const override {
        return "&" + profession.get_debug_name();
      }

      Profession &get_base() override {
        return profession.get_base();
      }

      const Profession &get_base() const override {
        return profession.get_base();
      }

      const Profession &get_profession() const {
        return profession;
      }

      virtual bool is_pointer() const {
        return false;
      }
  };

  class Pointer : public Reference {
  public:
      Pointer(Profession &profession) : Reference(profession) {}

      const std::string get_debug_name() const override {
        return "*" + profession.get_debug_name();
      }

      bool is_pointer() const override {
        return true;
      }
  };

//  bool profession_is_pointer(const Profession &profession) {
//    return profession.get_type() == Profession_Type::reference
//           && dynamic_cast<const Reference &>(profession).is_pointer();
//  }
}