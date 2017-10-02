#pragma once

#include <overworld/schema/professions/professions.h>
#include <overworld/schema/professions/Profession_Library.h>
#include "Expression.h"

namespace overworld {

  class Literal : public Common_Expression {

  public:
      Literal(const source_mapping::Source_Range &source_range) : Common_Expression(source_range) {}

      Type get_type() const override {
        return Type::literal;
      }

      virtual Primitive_Type get_primitive_type() const = 0;

      Profession &get_profession() override {
        return Profession_Library::get_primitive(get_primitive_type());
      }

      const Profession &get_profession() const override {
        return Profession_Library::get_primitive(get_primitive_type());
      }
  };

  template<typename T, typename Static_Functions>
  class Literal_Implementation : public Literal {
      T value;
      Element_Reference_Node node;

  public:
      Literal_Implementation(T value, Dungeon_Interface *dungeon, const source_mapping::Source_Range &source_range,
                             Function_Interface *function) :
        Literal(source_range),
        value(value),
        node(element, dungeon, function) {
        node.set_resolved(true);
      }

      Primitive_Type get_primitive_type() const override {
        return Static_Functions::_get_primitive_type();
      }

      ~Literal_Implementation() override {}

      const T &get_value() const {
        return value;
      }

      Node *get_node() override {
        return &node;
      }

      void set_profession(Profession &value) override {
        throw std::runtime_error("Not supported.");
      }
  };

  class Literal_Int : public Literal_Implementation<int, Literal_Int> {

  public:
      Literal_Int(int value, Dungeon_Interface *dungeon, const source_mapping::Source_Range &source_point,
                  Function_Interface *function) :
        Literal_Implementation(value, dungeon, source_point, function) {}

      static Primitive_Type _get_primitive_type() {
        return Primitive_Type::Int;
      }

      const std::string get_name() const override {
        return "int";
      }
  };

  class Literal_String : public Literal_Implementation<const std::string, Literal_String> {
  public:
      Literal_String(const std::string &value, Dungeon_Interface *dungeon,
                     const source_mapping::Source_Range &source_point, Function_Interface *function)
        :
        Literal_Implementation(value, dungeon, source_point, function) {}

      static Primitive_Type _get_primitive_type() {
        return Primitive_Type::String;
      }

      const std::string get_name() const override {
        return "string";
      }
  };

  class Literal_Bool : public Literal_Implementation<bool, Literal_Bool> {
  public:
      Literal_Bool(const bool &value, Dungeon_Interface *dungeon, const source_mapping::Source_Range &source_point,
                   Function_Interface *function) :
        Literal_Implementation(value, dungeon, source_point, function) {}

      static Primitive_Type _get_primitive_type() {
        return Primitive_Type::Bool;
      }

      const std::string get_name() const override {
        return "bool";
      }
  };
}