#pragma once

#include <overworld/expressions/Member_Expression.h>
#include "Minion.h"

namespace overworld {

  class Temporary_Minion : public Minion {
      std::vector<Member_Expression *> expressions;

  public:
      Temporary_Minion(const std::string &name, Profession &profession,const underworld::Source_Point source_point) :
        Minion(name, profession,source_point) {}

      virtual ~Temporary_Minion() {}

      void add_expression(Member_Expression &expression) {
        expressions.push_back(&expression);
      }

//      const std::vector<Member_Expression *> &get_expressions() const {
//        return expressions;
//      }

      void replace_with(Minion &minion) {
        for(auto expression: expressions){
          expression->set_member(minion);
        }
      }
  };
}