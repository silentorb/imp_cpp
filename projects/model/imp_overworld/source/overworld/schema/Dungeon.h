#pragma once

#include <underworld/schema/Dungeon.h>
#include "Scope.h"
#include "File.h"
#include <vector>

namespace overworld {

  class Dungeon;

  using Dungeon_Owner = std::unique_ptr<Dungeon>;
  using Dungeons = std::vector<Dungeon_Owner>;

  class Dungeon : public Scope, public virtual Profession, public virtual Profession_Reference {
      File *header_file = nullptr;
      Profession_Node<Dungeon> node;

  public:
      Dungeon(const underworld::Scope *source, Scope *parent) :
        Scope(source, parent), header_file(header_file), node(*this) {}

      virtual ~Dungeon() {

      }

      virtual const std::string get_name() const = 0;

      void set_file(File *value) {
        header_file = value;
      }

      File *get_file() const {
        return header_file;
      }

      Type get_type() const override {
        return Type::dungeon;
      }

      Scope *get_scope() override {
        return parent;
      }

      const Scope *get_scope() const {
        return parent;
      }

      Node &get_node() {
        return node;
      }

      void set_profession(const Profession &value) override {

      }

      const Profession &get_profession() const override {
        return *this;
      }

      Dungeon &get_dungeon() override{
        return *this;
      }

      const underworld::Source_Point &get_source_point() override {
        throw std::runtime_error("Not supported.");
      }

      Function & get_constructor();
  };

  class Derived_Dungeon : public Dungeon {
      const underworld::Dungeon &source;

  public:
      Derived_Dungeon(const underworld::Dungeon &source, Scope &parent) :
        source(source), Dungeon(&source, &parent) {}

      Derived_Dungeon(const underworld::Dungeon &source) :
        source(source), Dungeon(&source, parent) {}

      virtual ~Derived_Dungeon() {

      }

      const std::string get_name() const override {
        return source.get_name();
      }
  };

  class Anonymous_Dungeon : public Dungeon {
  public:
      Anonymous_Dungeon() :
        Dungeon(nullptr, nullptr) {}

      const std::string get_name() const override {
        return "Anonymous Dungeon";
      }
  };

  using Dungeon_Pointer = std::unique_ptr<Dungeon>;
}