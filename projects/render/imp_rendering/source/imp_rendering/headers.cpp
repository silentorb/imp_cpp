#include "headers.h"
#include <overworld/schema/Function.h>
#include <imp_artisan/building.h>

using namespace imp_artisan::building;
using namespace overworld;

using namespace std;

namespace imp_rendering {
  namespace headers {

    class Class_Block : public imp_artisan::internal::Standard_Block {
    public:
        Class_Block(const string &header) : Standard_Block(header, 0) {}

        const string get_end() const override {
          return "};";
        }
    };

    class Whitespace_Block : public imp_artisan::internal::Block {
        std::string header;

    public:
        Whitespace_Block(const string &header) : header(header) {}

        const string get_header() const override {
          return header;
        }

        const string get_start() const override {
          return "";
        }

        int get_indent() const override {
          return 2;
        }

        const string get_end() const override {
          return "";
        }
    };

    class Simple_Block : public imp_artisan::internal::Block {

    public:
        Simple_Block() {}

        const string get_header() const override {
          return "";
        }

        const string get_start() const override {
          return "";
        }

        int get_indent() const override {
          return 2;
        }

        const string get_end() const override {
          return "";
        }
    };

    Stroke render_dungeon_body(const overworld::Dungeon &dungeon) {
      Stroke block(new Class_Block("class " + dungeon.get_name()));
      Stroke private_block(new Simple_Block());
      Stroke public_block(new Whitespace_Block("public:"));

      for (auto &minion : dungeon.get_minions()) {
        auto assignment = minion->get_profession().get_type() == Profession_Type::reference
                          ? " = nullptr"
                          : "";

        auto stroke = Stroke(render_minion_with_signature(*minion) + assignment + ';');
        private_block.add(stroke);
        //          auto &profession = reflect_profession(dungeon_minion.get_profession());
//          auto &output_minion = output.create_minion(dungeon_minion, profession);

      }

      for (auto &function : dungeon.get_functions()) {
        if (function->is_inline()) {
          public_block << render_function_definition(*function);
        }
        else {
          public_block << render_function_declaration(*function) + ";";
        }
      }

      block.add(private_block);
      block.add(public_block);
      return block;
    }

    Stroke render_forward_declarations(const vector<const Profession *> &forward_declarations) {
      Stroke result;
      for (auto declaration : forward_declarations) {
        result << "class " + declaration->get_name() + ";";
      }
      return result;
    }

    Stroke render(const overworld::Dungeon &dungeon, const std::vector<overworld::File *> &files,
                  const vector<const Profession *> &forward_declarations) {
//      return Stroke()
//        << (Stroke() << "#pragma once")
//                     << render_dungeon_body(dungeon);
      Stroke result;
      result << wrap("#pragma once");

      if (files.size() > 0)
        result << render_includes(files);

      if (forward_declarations.size() > 0)
        result << render_forward_declarations(forward_declarations);

      result << render_dungeon_body(dungeon);

      return result;
    }
  }
}