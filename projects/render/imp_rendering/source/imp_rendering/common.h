#pragma once

#include <imp_artisan/Stroke.h>
#include <overworld/schema/Dungeon.h>
#include <overworld/expressions/Block.h>

namespace imp_rendering {

  using Stroke = imp_artisan::building::Stroke_Owner;
  const std::string render_function_parameters(const overworld::Function &function);
  const std::string render_function_return_signature(const overworld::Function &function);
  const std::string render_function_declaration(const overworld::Function &function);
  const std::string render_profession(const overworld::Profession &profession);
  const std::string render_expression(const overworld::Expression &input_expression);
  void render_statements(Stroke &stroke, const overworld::Expressions &statements, const overworld::Scope &scope);
  Stroke render_block(const std::string &header, const overworld::Block &block, const std::string &footer = "}");
  Stroke render_statement(const overworld::Expression &input_expression, const overworld::Scope &scope);

  inline Stroke wrap(const std::string &text) {
    Stroke result;
    result << text;
    return result;
  }
}