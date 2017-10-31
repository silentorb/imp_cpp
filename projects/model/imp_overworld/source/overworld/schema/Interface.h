#pragma once

#include "Temporary_Interface.h"

namespace overworld {

  class Virtual_Function;

  class Interface {
      std::vector<std::unique_ptr<Virtual_Function>> functions;
      Member_Collection member_collection;

  public:
      virtual bool is_temporary() const {
        return false;
      }

      std::map<std::string, Member> &get_members() {
        return member_collection.get_members();
      }
  };


}