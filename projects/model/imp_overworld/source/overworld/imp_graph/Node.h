#pragma once

#include <solving/Node.h>
#include <overworld/schema/professions.h>

namespace overworld {

  class Node : public solving::Node {
  public:
      virtual const Profession &get_profession() = 0;
  };

  template<typename T>
  class Profession_Node : public Node {
      T &element;

  public:
      Profession_Node(T &element) : element(element) {}

      const Profession &get_profession() override {
        return element.get_profession();
      }
  };

}