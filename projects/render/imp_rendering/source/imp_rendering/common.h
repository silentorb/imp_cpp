#pragma once

#include <imp_artisan/Stroke.h>
#include <overworld/schema/Dungeon.h>

namespace imp_rendering {

  using Stream = imp_artisan::Stroke_Stream;
  const std::string render_function_parameters(const overworld::Function &function);
  const std::string render_function_return_signature(const overworld::Function &function);
  const std::string render_function_declaration(const overworld::Function &function);

}