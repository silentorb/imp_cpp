#include "Artisan.h"

using namespace std;

namespace imp_artisan {

  std::string Artisan::render_block(const internal::Block &block, const Indent &indent) {
    auto new_indent = block.get_indent() == 1
                      ? indent + tab
                      : indent + tab + tab;

    return block.get_header() + " {\n"
           + render_strokes(block.get_strokes(), new_indent)
           + "\n" + indent + block.get_footer();
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
        return indent.substr(tab.size() * 2) + stroke.get_value();
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
      if (is_paragraph(stroke) || is_paragraph(*last)) {
        result += "\n";
      }
      result += "\n" + render_stroke(stroke, indent);
      last = &stroke;
    }

    return result;
  }

  std::string Artisan::render(const building::Stroke_Owner &stroke) {
    return render_stroke(stroke, "");
  }
}