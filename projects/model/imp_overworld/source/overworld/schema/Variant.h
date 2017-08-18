#pragma once

#include <string>
#include <overworld/schema/professions/Profession.h>
#include <overworld/imp_graph/Node.h>

namespace overworld {

  class Variant {
  protected:
      std::vector<Profession *> professions;
      std::string name;
      std::vector<Node_Owner> nodes;

      const std::string &format_name(const std::string &original_name);

  public:
      Variant(const std::string &original_name, const std::vector<Profession *> &professions) :
        professions(professions) {
        format_name(original_name);
      }
  };

  bool professions_match(const std::vector<Profession *> &first, const std::vector<Profession *> &second);
}