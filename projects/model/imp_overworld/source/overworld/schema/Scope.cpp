#include <underworld/schema/Minion.h>
#include <underworld/schema/Function.h>
#include "Scope.h"
#include "exceptions.h"
#include "Function.h"

using namespace std;

namespace overworld {

  Scope::Scope(const underworld::Scope &source, Scope_Parent &parent) :
    source(source), parent(parent) {

  }

  Scope::~Scope() {

  }

  Function &Scope::create_function(const underworld::Function &input) {
//    check_has_member(name);

    auto function = new Function(input, *this);
    functions.push_back(unique_ptr<Function>(function));
    return *function;
  }

  Variable &Scope::create_minion(const underworld::Minion &input, const Profession &profession) {
//    check_has_member(name);

    auto minion = new Variable(input, profession);
    variables.push_back(unique_ptr<Variable>(minion));
    return *minion;
  }

  Variable &Scope::get_variable(const std::string &name) {
    for (auto &variable: variables) {
      if (variable->get_name() == name)
        return *variable;
    }

    throw std::runtime_error("Could not find variable named " + name);
  }
}