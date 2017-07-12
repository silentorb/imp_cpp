#pragma once

#include <string>
#include <memory>
#include "../Source_Point.h"

namespace underworld {

  class Profession {
  public:

      enum class Type {
          unknown,
          primitive,
          dungeon,
          dungeon_reference,
          reference,
          function,
          token,
      };

      virtual Type get_type() const = 0;
      virtual const std::string get_name() const = 0;
      virtual const Source_Point get_source_point() const = 0;
  };

  using Profession_Owner = std::unique_ptr<Profession>;

  class Token_Profession : virtual public Profession {
      const std::string name;
      const Source_Point source_point;

  public:
      Token_Profession(const std::string &name, const Source_Point &source_point) :
        name(name), source_point(source_point) {}

      Type get_type() const override {
        return Profession::Type::token;
      }

      const std::string get_name() const override {
        return name;
      }

      const Source_Point get_source_point() const override {
        return source_point;
      }
  };

  class Dungeon_Reference_Profession : virtual public Profession {
      const std::string name;
      Profession_Owner child;
      const Source_Point source_point;

  public:
      Dungeon_Reference_Profession(const std::string &name, Profession_Owner &child,
                                   const Source_Point &source_point) :
        name(name), child(std::move(child)), source_point(source_point) {}

      Type get_type() const override {
        return Profession::Type::dungeon_reference;
      }

      const std::string get_name() const override {
        return name;
      }

      const Source_Point get_source_point() const override {
        return source_point;
      }

      const Profession &get_child() const {
        return *child;
      }
  };
}

