#pragma once

#include <underworld/schema/Dungeon.h>
#include "Scope.h"
#include "File.h"
#include <vector>

namespace overworld {

  class Dungeon;

  using Dungeon_Owner = std::unique_ptr<Dungeon>;
  using Dungeons = std::vector<Dungeon_Owner>;

  class Dungeon : public Scope, public virtual Profession, public virtual Profession_Reference,
                  public virtual Member {
      File *header_file = nullptr;
      Profession_Node<Dungeon> node;
      const std::string name;
      const underworld::Source_Point source_point;
      bool _is_external = false;
      Ownership default_ownership = Ownership::owner;

  public:
      Dungeon(const std::string &name, Scope &parent) :
        name(name), Scope(nullptr, &parent), node(*this) {}

      Dungeon(const std::string &name) :
        name(name), Scope(nullptr, nullptr), node(*this) {}

      virtual ~Dungeon() {}

      const std::string get_name() const {
        return name;
      }

      virtual const std::string get_cpp_name() const {
        return name;
      }

      bool is_class() const {
        return get_minions().size() > 0;
      }

      void set_file(File *value) {
        header_file = value;
      }

      File *get_file() const {
        return header_file;
      }

      Profession_Type get_type() const override {
        return Profession_Type::dungeon;
      }

      Scope *get_scope() override {
        return parent;
      }

      const Scope *get_scope() const {
        return parent;
      }

      virtual Node &get_node() override {
        return node;
      }

      void set_profession(const Profession &value) override {

      }

      const Profession &get_profession() const override {
        return *this;
      }

      Dungeon &get_dungeon() override {
        return *this;
      }

      const underworld::Source_Point &get_source_point() override {
        return source_point;
      }

      Function &get_or_create_constructor();

      Member_Type get_member_type() const override {
        return Member_Type::dungeon;
      }

      bool is_external() const {
        return _is_external;
      }

      void set_is_external(bool value) {
        _is_external = value;
      }

      Dungeon &create_dungeon(const std::string &name);
      Dungeon &add_dungeon(std::unique_ptr<Dungeon> dungeon);

      void set_default_ownership(Ownership value) {
        this->default_ownership = value;
      }

      Ownership get_ownership() const override {
        return default_ownership;
      }
  };

  class Cpp_Dungeon : public Dungeon {
      std::string cpp_name;

  public:
      Cpp_Dungeon(const std::string &name, const std::string &cpp_name, Scope &parent) :
        Dungeon(name, parent), cpp_name(cpp_name) {}

      const std::string get_cpp_name() const override {
        return cpp_name;
      }
  };

  using Dungeon_Pointer = std::unique_ptr<Dungeon>;
}