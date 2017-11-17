#pragma once

#include <memory>
#include "Member.h"
#include "overworld/schema/professions/Profession.h"
#include "Enchantment.h"
#include "Enchantment_Container.h"
#include "Variable.h"
#include <underworld/schema/Minion.h>
#include <overworld/imp_graph/Node.h>

namespace overworld {

  class Dungeon;

  class Simple_Minion {
  protected:
      const std::string name;
      Profession *profession;

  public:
      Simple_Minion(const std::string &name, Profession &profession) :
        name(name), profession(&profession) {}


      const std::string get_name() const {
        return name;
      }

      const Profession &get_profession() const {
        return *profession;
      }

      Profession &get_profession() {
        return *profession;
      }

      virtual void set_profession(Profession &value) {
        profession = &value;
      }
  };

  class Minion : public Variable {
      File *header_file = nullptr;

  public:
      Minion(const std::string &name, Profession_Reference &profession, Parent parent,
             const source_mapping::Source_Range &source_point) :
        Variable(Element_Type::minion, name, profession, parent, source_point) {}

      Minion(const Minion &) = delete;

      virtual ~Minion() {}

      const File *get_file() const {
        return header_file;
      }

      void set_file(File *value) {
        header_file = value;
      }
  };

  using Minion_Owner = std::unique_ptr<Minion>;
  using Simple_Minion_Owner = std::unique_ptr<Simple_Minion>;

  overworld::Minion &find_member_container(overworld::Expression &expression);
}