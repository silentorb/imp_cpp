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

        bool is_paragraph() const override {
          return false;
        }
    };

    std::string render_dungeon_definition_header(const overworld::Dungeon &dungeon) {
      string ancestors = "";
      if (dungeon.get_base_dungeon()) {
        ancestors = " : public " + get_cpp_name(*dungeon.get_base_dungeon());
      }
      return "class " + get_cpp_name(dungeon.get_name()) + ancestors;
    }

    Stroke render_function_declaration_or_definition(const Function &function) {
      if (function.is_inline()) {
        return render_function_definition(function);
      }
      else {
        return render_function_declaration(function) + ";";
      }
    }

    Stroke render_minion(const Minion &minion, const Dungeon &dungeon) {
      auto assignment = minion.get_profession().get_type() == Profession_Type::reference
                        ? " = nullptr"
                        : "";

      return Stroke(render_minion_with_signature(minion, *dungeon.get_scope()) + assignment + ';');
    }

    Stroke render_dungeon_body(const overworld::Dungeon &dungeon) {
      Stroke block(new Class_Block(render_dungeon_definition_header(dungeon)));
      Stroke private_block(new Simple_Block());
      Stroke public_block(new Whitespace_Block("public:"));

      for (auto &minion : dungeon.get_minions()) {
        if (minion->has_enchantment(Enchantment_Library::get_public())) {
          public_block << render_minion(*minion, dungeon);
        }
        else {
          private_block << render_minion(*minion, dungeon);
        }
      }

      for (auto &function : dungeon.get_functions()) {
        if (function->has_enchantment(Enchantment_Library::get_public())) {
          public_block << render_function_declaration_or_definition(*function);
        }
        else {
          private_block << render_function_declaration_or_definition(*function);
        }
      }

      block.add(private_block);

      if (dungeon.get_minions().size() + dungeon.get_functions().size() > 0) {
        block.add(public_block);
      }
      return block;
    }

    Stroke render_forward_declarations(const vector<const Profession *> &forward_declarations) {
      Stroke result;
      for (auto declaration : forward_declarations) {
        result << "class " + declaration->get_name() + ";";
      }
      return result;
    }

    Stroke render(const overworld::Dungeon &dungeon, const std::vector<overworld::File_Reference> &files,
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

      result << render_possible_generic_block(dungeon.get_generic_parameters(), render_dungeon_body(dungeon));

      return result;
    }
  }
}