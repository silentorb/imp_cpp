#pragma once

#include "Member.h"

namespace overworld {

  class Member_Profession : public Generic_Member_Reference<Profession> {
  public:
      explicit Member_Profession(Profession &parameter) :
        Generic_Member_Reference(parameter) {}

      Member_Type get_member_type() const override {
        return Member_Type::profession;
      }

      Profession & get_profession(){
        return value;
      }
  };

}