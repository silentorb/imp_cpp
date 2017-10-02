#pragma once

#include <string>
#include <memory>
#include "source_mapping/Source_Point.h"

namespace underworld {

  enum class Profession_Type {
      unknown,
      primitive,
      dungeon,
      dungeon_reference,
      Enum,
      function,
      pointer,
      reference,
      token,
      variant,
  };

  class Profession {
  public:

      virtual Profession_Type get_type() const = 0;
      virtual const std::string get_name() const = 0;
      virtual const source_mapping::Source_Range get_source_point() const = 0;
  };

  using Profession_Owner = std::unique_ptr<Profession>;

  class Token_Profession : virtual public Profession {
      const std::string name;
      const source_mapping::Source_Range source_point;

  public:
      Token_Profession(const std::string &name, const source_mapping::Source_Range &source_point) :
        name(name), source_point(source_point) {}

      Profession_Type get_type() const override {
        return Profession_Type::token;
      }

      const std::string get_name() const override {
        return name;
      }

      const source_mapping::Source_Range get_source_point() const override {
        return source_point;
      }
  };

  class Unknown : virtual public Profession {
      const source_mapping::Source_Range source_point;

  public:
      Unknown(const source_mapping::Source_Range &source_point) :
        source_point(source_point) {}

      Profession_Type get_type() const override {
        return Profession_Type::unknown;
      }

      const std::string get_name() const override {
        return "unknown";
      }

      const source_mapping::Source_Range get_source_point() const override {
        return source_point;
      }
  };

  class Dungeon_Reference_Profession : public Profession {
      const std::string name;
      Profession_Owner child;
      const source_mapping::Source_Range source_point;

  public:
      Dungeon_Reference_Profession(const std::string &name, Profession_Owner &child,
                                   const source_mapping::Source_Range &source_point) :
        name(name), child(std::move(child)), source_point(source_point) {}

      Profession_Type get_type() const override {
        return Profession_Type::dungeon_reference;
      }

      const std::string get_name() const override {
        return name;
      }

      const source_mapping::Source_Range get_source_point() const override {
        return source_point;
      }

      const Profession &get_child() const {
        return *child;
      }
  };
}

