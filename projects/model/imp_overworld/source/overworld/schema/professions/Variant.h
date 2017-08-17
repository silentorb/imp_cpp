#pragma once

#include <overworld/imp_graph/Node.h>

namespace overworld {

  class Variant {
      std::vector<Node_Owner> nodes;
      Node *dungeon_node = nullptr;
      std::vector<Profession *> professions;

  public:
      Variant(const std::vector<Profession *> &professions) : professions(professions) {}


      void set_dungeon_node(Node &value) {
        dungeon_node = &value;
      }

      void add_node(Node_Owner node) {
        nodes.push_back(std::move(node));
      }


  };

  bool professions_match(const std::vector<Profession *> &first, const std::vector<Profession *> &second);

  using Variant_Owner = std::unique_ptr<Variant>;
}