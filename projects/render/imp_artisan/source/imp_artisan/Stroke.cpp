#include "Stroke.h"

static const char *newline_a = "\n";
static const char *newline_b = "\n";
static const char *newline_c = "\n";
static const char *newline_d = "\n";
static const char *newline_e = "\n";

namespace imp_artisan {
  namespace internal {

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

    std::string Block::render(const Indent &indent)const {
      auto new_indent = indent;
      for (int i = 0; i < get_indent(); ++i) {
        new_indent += tab_string;
      }

      auto &strokes = get_strokes();

      auto block_start = get_start() != ""
                         ? " " + get_start()
                         : get_start();

//    if (strokes.size() == 1 && !is_paragraph(strokes[0])) {
//      return indent + get_header()// + newline_a
//             + render_strokes(strokes, new_indent);
//    }
//    else
      if (strokes.size() == 0) {
        if (get_start() == "")
          return indent + get_header();
        else
          return indent + get_header() + block_start + get_end();
      }

      else {

        auto block_end = get_end() != ""
                         ? newline_b + indent + get_end()
                         : get_end();

        auto opening = get_header() + block_start;
        auto opening2 = opening == "" ? "" : opening + newline_c;
        return indent + opening2
               + render_strokes(strokes, new_indent)
               + block_end;
      }
    }

    std::string Group::render(const Indent &indent) const {
      return render_strokes(strokes, indent);
    }

  }

  std::string Tight_Group::render(const Indent &indent) const {
    return internal::render_strokes_tight(strokes, indent);
  }
}