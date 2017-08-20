#include "Artisan.h"

//#define DEBUGNEWLINES

#ifndef DEBUGNEWLINES
static const char *newline_a = "\n";
static const char *newline_b = "\n";
static const char *newline_c = "\n";
static const char *newline_d = "\n";
static const char *newline_e = "\n";
#else
static const char * newline_a = "a\n";
static const char * newline_b = "b\n";
static const char * newline_c = "c\n";
static const char * newline_d = "d\n";
static const char * newline_e = "e\n";
#endif

using namespace std;

namespace imp_artisan {
/*
  std::string Artisan::render_block(const internal::Block &block, const Indent &indent) {
    auto new_indent = indent;
    for (int i = 0; i < block.get_indent(); ++i) {
      new_indent += tab;
    }

    auto &strokes = block.get_strokes();

    auto block_start = block.get_start() != ""
                       ? " " + block.get_start()
                       : block.get_start();

//    if (strokes.size() == 1 && !is_paragraph(strokes[0])) {
//      return indent + block.get_header()// + newline_a
//             + render_strokes(strokes, new_indent);
//    }
//    else
    if (strokes.size() == 0) {
      if (block.get_start() == "")
        return indent + block.get_header();
      else
        return indent + block.get_header() + block_start + block.get_end();
    }

    else {

      auto block_end = block.get_end() != ""
                       ? newline_b + indent + block.get_end()
                       : block.get_end();

      auto opening = block.get_header() + block_start;
      auto opening2 = opening == "" ? "" : opening + newline_c;
      return indent + opening2
             + render_strokes(strokes, new_indent)
             + block_end;
    }
  }

  std::string Artisan::render_stroke(const building::Stroke_Owner &stroke, const Indent &indent) {
    switch (stroke.get_type()) {
      case internal::Stroke::Type::block:
        return render_block(*dynamic_cast<const internal::Block *>(stroke.get_pointer()), indent);

      case internal::Stroke::Type::group:
        return render_strokes((*dynamic_cast<const internal::Group *>(stroke.get_pointer())).get_strokes(), indent);

      case internal::Stroke::Type::text:
        return indent + stroke.get_value();

      case internal::Stroke::Type::special:
        return indent.substr(tab.size()) + stroke.get_value();
    }

    throw runtime_error("Unsupported stroke type.");
  }

  std::string Artisan::render_strokes(const building::Strokes &strokes, const Indent &indent) {
    if (strokes.size() == 0)
      return "";

    const building::Stroke_Owner *last = &strokes[0];
    string result = render_stroke(*last, indent);
    for (auto it = strokes.begin() + 1; it != strokes.end(); it++) {
      auto &stroke = (*it);
      if (stroke.is_paragraph() || last->is_paragraph()) {
        result += newline_d;
      }
      result += newline_e + render_stroke(stroke, indent);
      last = &stroke;
    }

    return result;
  }

  std::string Artisan::render(const building::Stroke_Owner &stroke) {
    return render_stroke(stroke, "");
  }
  */
}