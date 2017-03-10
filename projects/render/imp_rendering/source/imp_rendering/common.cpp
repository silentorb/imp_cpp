#include "common.h"
#include <overworld/schema/Function.h>

using namespace std;

namespace imp_rendering {

  const std::string render_function_parameters(const overworld::Function &function) {
    return "()";
  }

  const std::string render_function_return_signature(const overworld::Function &function) {
    string return_text = function.is_constructor()
                         ? ""
                         : "void ";

    return return_text;
  }

  const std::string render_function_declaration(const overworld::Function &function) {
    return render_function_return_signature(function)
           + function.get_name()
           + render_function_parameters(function);
  }
}