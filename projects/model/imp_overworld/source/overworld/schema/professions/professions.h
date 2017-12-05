#pragma once

#include <memory>
#include <source_mapping/Source_Point.h>
#include <overworld/schema/Dungeon.h>
#include "Profession.h"

namespace overworld {

  class Unknown : public Profession {
  public:
      Profession_Type get_type() const override {
        return Profession_Type::unknown;
      }

//      Scope *get_scope() override {
//        return nullptr;
//      }
//
//      const Scope *get_scope() const override {
//        return nullptr;
//      }

      File *get_file() const override {
        return nullptr;
      }

      const std::string get_name() const override {
        return "?";
      }
  };

  class Void : public Profession {
  public:
      Profession_Type get_type() const override {
        return Profession_Type::Void;
      }

//      Scope *get_scope() override {
//        return nullptr;
//      }
//
//      const Scope *get_scope() const override {
//        return nullptr;
//      }

      File *get_file() const override {
        return nullptr;
      }

      const std::string get_name() const override {
        return "void";
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

  class Primitive : public Profession {
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

//      Scope *get_scope() override {
//        return nullptr;
//      }

//      const Scope *get_scope() const override {
//        return nullptr;
//      }

      File *get_file() const override {
        return file;
      }

      const std::string get_name() const override;
  };

//  enum class Reference_Type {
//      none,
//      owner,
//      pointer,
//      reference,
//  };

  class Reference : public Profession {
  protected:
      Profession_Reference profession;
      Ownership_Storage info;

  public:
      Reference(Ownership_Storage info, Profession_Reference &profession) :
        info(info), profession(profession) {}

      virtual ~Reference() {}

      Profession_Type get_type() const override {
        return Profession_Type::reference;
      }

      File *get_file() const override {
        return nullptr;
      }

      Ownership get_ownership() const {
        return info.ownership;
      }

      const std::string get_name() const override {
        return profession.get_name();
      }

      const char get_symbol() const {
        static char symbols[] = {'?', '$', '*', '&'};
        return symbols[(int) info.storage];
      }

      const std::string get_debug_name() const override {
        return get_symbol() + profession->get_debug_name();
      }

      Profession_Reference &get_base(Profession_Reference &self) override {
        return profession.get_base(profession);
      }

      const Profession &get_base() const override {
        return profession.get_base();
      }

      const Profession_Reference &get_profession() const {
        return profession;
      }

      const Ownership_Storage &get_info() const {
        return info;
      }

      Storage_Type get_storage() const {
        return info.storage;
      }

  };

//  class Pointer : public Reference {
//  public:
//      Pointer(Profession_Reference &profession) : Reference(profession) {}
//
//      const std::string get_debug_name() const override {
//        return "*" + profession->get_debug_name();
//      }
//
//      bool is_pointer() const override {
//        return true;
//      }
//  };
}