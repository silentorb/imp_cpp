#include "Context.h"

namespace imp_summoning {

  underworld::Member *Context::find_member_internal(const std::string &name) const {
    auto &scope_members = scope.get_members();
    auto search = scope_members.find(name);
    if (search != scope_members.end()) {
      return (*search).second.get();
    }
    return nullptr;
  }

  underworld::Member *Child_Context::find_member(const std::string &name) const {
    auto result = find_member_internal(name);
    return result
           ? result
           : parent.find_member(name);
  }
}