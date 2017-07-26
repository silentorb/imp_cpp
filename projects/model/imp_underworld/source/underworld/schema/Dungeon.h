#pragma once

#include "Scope.h"

namespace underworld {

  class Dungeon : public Scope, public virtual Profession {
      const std::string name;

  protected:
      void throw_already_exists(const std::string &member_name) const override;

  public:
      Dungeon(const std::string &name, Scope *parent);
      virtual ~Dungeon();

      const std::string get_name() const {
        return name;
      }

      Profession_Type get_type() const override {
        return Profession_Type::dungeon;
      }

      Dungeon &get_dungeon() override{
        return *this;
      }

      const Source_Point get_source_point() const override {
        throw new std::runtime_error("Not implemented.");
      }
  };

//  using Dungeons = std::map<std::string, std::unique_ptr<Dungeon>>;
  using Dungeon_Pointer = std:: unique_ptr<Dungeon>;
}