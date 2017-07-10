#pragma once

#include <string>
#include <memory>

namespace underworld {

  class Profession {
  public:

      enum class Type {
          unknown,
          primitive,
          dungeon,
          reference,
          function,
          token,
      };

      virtual Type get_type() const = 0;
      virtual const std::string get_name() const = 0;
  };

  using Profession_Owner = std::unique_ptr<Profession>;

  class Token_Profession : virtual public Profession {
      const std::string name;

  public:
      Token_Profession(const std::string &name) : name(name) {}

      Type get_type() const override {
        return Profession::Type::token;
      }

      const std::string get_name() const override {
        return name;
      }
  };

  class Dungeon_Reference_Profession : virtual public Profession {
      const std::string name;
      Profession_Owner child;

  public:
      Dungeon_Reference_Profession(const std::string &name, Profession_Owner &child) :
        name(name), child(std::move(child)) {}

      Type get_type() const override {
        return Profession::Type::dungeon;
      }

      const std::string get_name() const override {
        return name;
      }
  };
}

