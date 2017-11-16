#pragma once

#include <string>
#include <memory>
#include "source_mapping/Source_Point.h"

namespace underworld {

  enum class Profession_Type {
      unknown,
      primitive,
      dungeon,
//      dungeon_reference,
      Enum,
      function,
      pointer,
      decorator,
      token,
      variant,
  };

  class Profession {
  public:

      virtual Profession_Type get_type() const = 0;
      virtual const std::string get_name() const = 0;
      virtual const source_mapping::Source_Range get_source_point() const = 0;

      virtual ~Profession() {}
  };

  using Profession_Owner = std::unique_ptr<Profession>;

  class Token_Profession : public Profession {
      const std::string name;
      const source_mapping::Source_Range source_point;
      Profession_Owner child;

  public:
           Token_Profession(const std::string &name, Profession_Owner child,
                       const source_mapping::Source_Range &source_point) :
        name(name), child(std::move(child)), source_point(source_point) {}

      Token_Profession(const std::string &name,
                       const source_mapping::Source_Range &source_point) :
        name(name), source_point(source_point) {}

      Profession_Type get_type() const override {
        return Profession_Type::token;
      }

      const std::string get_name() const override {
        return name;
      }
      const Profession *get_child() const {
        return child.get();
      }
      const source_mapping::Source_Range get_source_point() const override {
        return source_point;
      }
  };

  class Unknown : public Profession {
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

//  class Dungeon_Reference_Profession : public Profession {
//      const std::string name;
//      Profession_Owner child;
//      const source_mapping::Source_Range source_point;
//
//  public:
//      Dungeon_Reference_Profession(const std::string &name, Profession_Owner child,
//                                   const source_mapping::Source_Range &source_point) :
//        name(name), child(std::move(child)), source_point(source_point) {}
//
//      Profession_Type get_type() const override {
//        return Profession_Type::dungeon_reference;
//      }
//
//      const std::string get_name() const override {
//        return name;
//      }
//
//      const source_mapping::Source_Range get_source_point() const override {
//        return source_point;
//      }
//
//      const Profession &get_child() const {
//        return *child;
//      }
//  };

  class Function_Profession : public Profession {
      const source_mapping::Source_Range source_point;
      std::vector<Profession_Owner> elements;

  public:
      explicit Function_Profession(const source_mapping::Source_Range &source_point) : source_point(source_point) {}

      Function_Profession(Profession_Owner first, Profession_Owner second) :
        source_point(source_mapping::Source_Range(
          first->get_source_point().get_start(),
          second->get_source_point().get_end())
        ) {
        elements.push_back(std::move(first));
        elements.push_back(std::move(second));
      }

      Profession_Type get_type() const override {
        return Profession_Type::function;
      }

      const std::string get_name() const override {
        return "Function Signature";
      }

      const source_mapping::Source_Range get_source_point() const override {
        return source_point;
      }

      void add_element(Profession_Owner element) {
        elements.push_back(std::move(element));
      }

      const std::vector<Profession_Owner> &get_elements() const {
        return elements;
      }
  };
}

