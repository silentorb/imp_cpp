#pragma once

#include "Scope.h"

namespace underworld {

  using Dungeons = std::map<std::string, std::unique_ptr<Dungeon>>;

  class Dungeon : public Scope, public virtual Profession {
      const std::string name;
      bool _is_complete = true;

  protected:
      void throw_already_exists(const std::string &member_name) const override;

  public:
      Dungeon(const std::string &name, Scope *parent);
      virtual ~Dungeon();

      bool is_complete() const {
        return _is_complete;
      }

      void set_is_complete(bool value) {
        _is_complete = value;
      }

      const std::string get_name() const {
        return name;
      }

      Type get_type() const override {
        return Type::dungeon;
      }

      Dungeon &get_dungeon() override{
        return *this;
      }
  };

  using Dungeon_Pointer = std:: unique_ptr<Dungeon>;
}