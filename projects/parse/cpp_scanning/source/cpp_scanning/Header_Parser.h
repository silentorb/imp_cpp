#pragma once

#include <runic/common/Stream.h>
#include <runic_cpp/Cpp_Lexicon.h>
#include "Dungeon_Map.h"
#include "Context.h"
#include "Parser_Lookup.h"

namespace underworld {
  class Dungeon_Variant;
}

namespace cpp_scanning {

  class Header_Parser {
      Dungeon_Map &dungeons;
      runic::Stream &input;
      const runic_cpp::Symbols &lexicon;
      Parser_Lookup &lookup;

      void skip_to(const runic::Whisper &whisper);
      void skip_line();
      void skip_scope();
      void process_function_parameters(underworld::Function &function, Context &context);
      void process_variant_arguments(underworld::Dungeon_Variant &variant, Context &context);
      bool process_class_or_namespace_member(Context &context);
      underworld::Profession_Owner process_profession_token(Context &context);
      underworld::Profession_Owner process_profession_internal(Context &context);
      underworld::Profession_Owner process_profession(Context &context);
      void process_class_or_struct(Context &context, Access_Mode access_mode);
      underworld::Function &process_common_function_declaration(Context &context);
      void process_constructor(Context &context);
      void process_destructor(Context &context);
      void process_function_or_variable(Context &context);
      void process_namespace_member(Context &context);
      void process_class_member(Context &context);
      void process_child_namespace(Context &context);
      void process_class_namespace(Context &context);
      void process_root(Context &context);

      underworld::Dungeon &get_or_create_dungeon(const std::string &name, Context &context);
      underworld::Dungeon &create_dungeon(const std::string &name, Context &context);

  public:
      Header_Parser(Dungeon_Map &dungeons, runic::Stream &input, const runic_cpp::Symbols &lexicon,
                    Parser_Lookup &lookup) :
        dungeons(dungeons), input(input), lexicon(lexicon), lookup(lookup) {}

      void parse(underworld::Dungeon &root_dungeon);
  };
}