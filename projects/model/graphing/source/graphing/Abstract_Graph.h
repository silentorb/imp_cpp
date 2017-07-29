#pragma once

namespace graphing {

  template<typename Node, typename Connection>
  class Abstract_Graph {
  public:
      virtual Connection &connect(Node &first, Node &second) = 0;
      virtual void disconnect(Node &first, Node &second) = 0;
      virtual void remove(Node & node) = 0;
  };
};