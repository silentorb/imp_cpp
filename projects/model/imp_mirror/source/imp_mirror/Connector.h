#pragma once

#include <overworld/imp_graph/Graph.h>
#include <overworld/schema/professions/Profession_Library.h>
#include <overworld/expressions/Invoke.h>
#include "Scope.h"

namespace imp_mirror {

  class Connector {
      overworld::Graph &graph;
      overworld::Profession_Library &profession_library;

      overworld::Function_Signature &get_function_signature(overworld::Expression &expression,
                                                            std::vector<overworld::Expression_Owner> &arguments,
                                                            Scope &scope);

  public:
      Connector(overworld::Graph &graph, overworld::Profession_Library &profession_library) :
        graph(graph), profession_library(profession_library) {}

      void connect_parameters_and_arguments(overworld::Invoke &invoke,
                                            const std::vector<underworld::Expression_Owner> &source_arguments,
                                            Scope &scope);

  };
}