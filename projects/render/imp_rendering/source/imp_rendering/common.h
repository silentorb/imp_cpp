#pragma once

#include <imp_artisan/Stroke.h>
#include <overworld/schema/Dungeon.h>

namespace imp_rendering {

  using Stroke = imp_artisan::building::Stroke_Owner;
  const std::string render_function_parameters(const overworld::Function &function);
  const std::string render_function_return_signature(const overworld::Function &function);
  const std::string render_function_declaration(const overworld::Function &function);
  const std::string render_profession(const overworld::Profession &profession);
  const std::string render_expression(const overworld::Expression &input_expression);
  void render_statements(Stroke &stroke, const overworld::Expressions &statements, const overworld::Scope &scope);

  inline Stroke wrap(const std::string &text) {
    Stroke result;
    result << text;
    return result;
  }
}