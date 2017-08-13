#pragma once

#include <overworld/schema/professions/professions.h>
#include <overworld/schema/professions/Profession_Library.h>
#include <underworld/expressions/Literal.h>
#include "Expression.h"

namespace overworld {

  class Literal : public Common_Expression {

  public:
      Type get_type() const override {
        return Type::literal;
      }

      virtual Primitive_Type get_primitive_type() const = 0;

      Profession &get_profession() const {
        return Profession_Library::get_primitive(get_primitive_type());
      }
  };

  template<typename T, typename Static_Functions>
  class Literal_Implementation : public virtual Literal, public virtual Profession_Reference {
      T value;
      Profession_Node <Literal_Implementation> node;
      const underworld::Literal &source;

  public:
      Literal_Implementation(T value, const underworld::Literal &source) :
        value(value),
        node(*this, Profession_Library::get_primitive(Static_Functions::_get_primitive_type())), source(source) {
        node.set_resolved(true);
      }

      Primitive_Type get_primitive_type() const override {
        return Static_Functions::_get_primitive_type();
      }

      virtual ~Literal_Implementation() {

      }

      const T &get_value() const {
        return value;
      }

      Node *get_node() override {
        return &node;
      }

      Profession &get_profession() override {
        return Literal::get_profession();
      }

      const Profession &get_profession() const override {
        return Literal::get_profession();
      }

      void set_profession(Profession &value) override {
        throw std::runtime_error("Not supported.");
      }

      const underworld::Source_Point &get_source_point() const override {
        return source.get_source_point();
      }

  };

  class Literal_Int : public Literal_Implementation<int, Literal_Int> {

  public:
      Literal_Int(int value, const underworld::Literal &source) :
        Literal_Implementation(value, source) {}

      static Primitive_Type _get_primitive_type() {
        return Primitive_Type::Int;
      }

      const std::string get_name() const override {
        return "int";
      }
  };

  class Literal_String : public Literal_Implementation<const std::string, Literal_String> {
  public:
      Literal_String(const std::string &value, const underworld::Literal &source) :
        Literal_Implementation(value, source) {}

      static Primitive_Type _get_primitive_type() {
        return Primitive_Type::String;
      }

      const std::string get_name() const override {
        return "string";
      }
  };

  class Literal_Bool : public Literal_Implementation<bool, Literal_Bool> {
  public:
      Literal_Bool(const bool &value, const underworld::Literal &source) :
        Literal_Implementation(value, source) {}

      static Primitive_Type _get_primitive_type() {
        return Primitive_Type::Bool;
      }

      const std::string get_name() const override {
        return "bool";
      }
  };
}