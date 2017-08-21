#include "sources.h"
#include <overworld/schema/Function.h>
#include <imp_artisan/building.h>

using namespace imp_artisan::building;
using namespace std;
using namespace overworld;

namespace imp_rendering {
  namespace sources {

//    Stroke render_includes(const overworld::Dungeon &dungeon, const std::vector<File *> &files) {
//      Stroke result;
//      result << "#include \"" + dungeon.get_name() + ".h\""
//      << render_includes(files);
//
//      return result;
//    }

    Stroke render_function(const overworld::Function &function, const overworld::Dungeon &dungeon) {
      auto function_signature = render_function_return_signature(function)
                                + dungeon.get_name() + "::"
                                + sanitize_name(function.get_name())
                                + render_function_parameters(function);

      return render_block(function_signature, function.get_block());
    }


    Stroke render(const overworld::Dungeon &dungeon, const std::vector<overworld::File_Reference> &files) {
      Stroke result;
      result << render_includes(files);
      for (auto &function : dungeon.get_functions()) {
        if (!function->is_inline())
          result << render_function(*function, dungeon);
      }

      return result;
    }

    bool needs_source_file(const overworld::Dungeon &dungeon) {
      for (auto &function :dungeon.get_functions()) {
        if (!function->is_inline())
          return true;
      }

      return false;
    }
  }
}