#include "Artisan.h"

using namespace std;

namespace imp_artisan {

  std::string Artisan::render_block(const Block &block, const Indent &indent) {
    return block.get_header() + " {\n"
           + render_strokes(block.get_strokes(), indent + tab)
           + "\n" + indent + block.get_footer();
  }

  std::string Artisan::render_stroke(const Stroke &stroke, const Indent &indent) {
    switch (stroke.get_type()) {
      case Stroke::Type::block:
        return render_block(*dynamic_cast<const Block *>(&stroke), indent);

      case Stroke::Type::group:
        return render_strokes((*dynamic_cast<const Group *>(&stroke)).get_strokes(), indent);

      case Stroke::Type::text:
        return indent + render_text(*dynamic_cast<const Text *>(&stroke));

      case Stroke::Type::special:
        return indent.substr(tab.size()) + (*dynamic_cast<const Special_Text *>(&stroke)).get_value();
    }

    throw runtime_error("Unsupported stroke type.");
  }

  std::string Artisan::render_strokes(const Strokes &strokes, const Indent &indent) {
    if (strokes.size() == 0)
      return "";

    Stroke *last = strokes[0].get();
    string result = render_stroke(*last, indent);
    for (auto it = strokes.begin() + 1; it != strokes.end(); it++) {
      auto &stroke = *(*it);
      if (is_paragraph(stroke) && is_paragraph(*last)) {
        result += "\n";
      }
      result += "\n" + render_stroke(stroke, indent);
      last = &stroke;
    }

    return result;
  }

  std::string Artisan::render(const Strokes &strokes) {
    return render_strokes(strokes, "");
  }
}