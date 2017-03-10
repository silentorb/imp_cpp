#pragma once

#include <string>
#include "Stroke.h"

namespace imp_artisan {

  using Indent = std::string;

  class Artisan {
      const std::string tab = "  ";

      std::string render_block(const Block &block, const Indent &indent);
      std::string render_strokes(const Strokes &strokes, const Indent &indent);
      std::string render_stroke(const Stroke &stroke, const Indent &indent);

      std::string render_text(const Text &text) {
        return text.get_value();
      }

      bool is_paragraph(const Stroke &stroke) {
        return stroke.get_type() == Stroke::Type::group || stroke.get_type() == Stroke::Type::block;
      }

  public:
      std::string render(const Strokes &strokes);
  };
}