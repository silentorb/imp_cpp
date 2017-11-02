#include "Stroke.h"

static const char *newline_a = "\n";
static const char *newline_b = "\n";
static const char *newline_c = "\n";
static const char *newline_d = "\n";
static const char *newline_e = "\n";

namespace imp_artisan {

  std::string render_strokes(const building::Strokes &strokes, const Indent &indent) {
    if (strokes.size() == 0)
      return "";

    const building::Stroke_Owner *last = &strokes[0];
    std::string result = last->render(indent);
    for (auto it = strokes.begin() + 1; it != strokes.end(); it++) {
      auto &stroke = (*it);
      if (stroke.is_paragraph() || last->is_paragraph()) {
        result += newline_d;
      }
      result += newline_e + stroke.render(indent);
      last = &stroke;
    }

    return result;
  }

  namespace internal {

    std::string render_strokes_tight(const building::Strokes &strokes, const Indent &indent) {
      if (strokes.size() == 0)
        return "";

      std::string result = strokes[0].render(indent);
      for (auto it = strokes.begin() + 1; it != strokes.end(); it++) {
        auto &stroke = (*it);
        result += newline_e + stroke.render(indent);
      }

      return result;
    }

    std::string Block::create_indent(int steps) const {
      std::string new_indent = "";
      for (int i = 0; i < steps; ++i) {
        new_indent += tab_string;
      }
      return new_indent;
    }

    const std::string Block::render_main(const Indent &indent, int steps, const std::string &opening,
                                         const std::string &closing) const {
      auto new_indent = indent + create_indent(steps);
      return indent + opening
             + render_strokes(strokes, new_indent)
             + closing;
    }

    std::string Group::render(const Indent &indent) const {
      return render_strokes(strokes, indent);
    }

    const std::string Standard_Block::render_standard(const Indent &indent, int steps) const {
      if (strokes.empty())
        return indent + header + "{ }";

      return render_main(
        indent, steps,
        header + " {" + newline_c,
        newline_b + indent + "}"
      );
    }

    std::string Standard_Block::render(const Indent &indent) const {
      return render_standard(indent, 1);
    }
  }

  std::string Tight_Group::render(const Indent &indent) const {
    return internal::render_strokes_tight(strokes, indent);
  }
}