#pragma once

#include <memory>
#include "Member.h"
#include "overworld/schema/professions/Profession.h"
#include <underworld/schema/Minion.h>
#include <overworld/imp_graph/Node.h>
#include <overworld/schema/professions/Profession_Reference.h>

namespace overworld {

  class Dungeon;

  class Minion : public virtual Member, public virtual Profession_Reference {
      const underworld::Minion *source;
      const Profession *profession;
      Profession_Node<Minion> node;
      const std::string name;
      const underworld::Source_Point source_point;

  public:
      Minion(const underworld::Minion &source, const Profession &profession) :
        source(&source), profession(&profession), node(*this), name(source.get_name()),
        source_point(source.get_source_point()) {}

//      Minion(const underworld::Minion &source) :
//        source(&source), profession(nullptr), node(*this), name(source.get_name()),
//        source_point(source.get_source_point()) {}

      Minion(const std::string &name, const Profession &profession) :
        source(nullptr), profession(&profession), node(*this), name(name) {}

      virtual ~Minion() {

      }

      Member_Type get_member_type() const override {
        return Member_Type::variable;
      }

      const std::string get_name() const override {
        return name;
      }

      const Profession &get_profession() const override {
        return *profession;
      }

      void set_profession(const Profession &value) override {
        profession = &value;
      }

      virtual const underworld::Source_Point &get_source_point() override {
        return source_point;
      }

      Node &get_node() {
        return node;
      }

      virtual bool is_parameter() const {
        return false;
      }
  };

  using Variable_Owner = std::unique_ptr<Minion>;

}