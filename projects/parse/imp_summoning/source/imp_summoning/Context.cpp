#include "Context.h"

namespace imp_summoning {

  underworld::Member *Child_Context::find_member_or_null(const std::string &name) const {
    auto &scope_members = scope.get_members();
    auto search = scope_members.find(name);
    if (search != scope_members.end()) {
      return (*search).second.get();
    }
    return parent.find_member_or_null(name);
  }
}