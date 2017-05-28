#pragma once

#include <underworld/schema/Dungeon.h>
#include "Scope.h"
#include "File.h"
#include <vector>

namespace overworld {

  class Dungeon;

  using Dungeon_Owner = std::unique_ptr<Dungeon>;
  using Dungeons = std::vector<Dungeon_Owner>;

  class Dungeon : public Scope, public virtual Profession {
      const underworld::Dungeon &source;
      File *header_file = nullptr;
//      Dungeon *parent = nullptr;

  public:
      Dungeon(const underworld::Dungeon &source, Scope &parent);
      Dungeon(const underworld::Dungeon &source);
//      Dungeon &create_dungeon(underworld::Dungeon &input_dungeon);

//      Dungeons &get_dungeons() {
//        return dungeons;
//      }
//
//      const Dungeons &get_dungeons() const {
//        return dungeons;
//      }

      const std::string get_name() const {
        return source.get_name();
      }

      void set_file(File *value) {
        header_file = value;
      }

      File *get_file() const {
        return header_file;
      }

//      Dungeon *get_parent() {
//        return parent;
//      }

      Type get_type() const override {
        return Type::dungeon;
      }

      Scope *get_scope() override {
        return parent;
      }

      const Scope *get_scope() const {
        return parent;
      }
  };
}