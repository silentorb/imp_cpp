#pragma once

#include <string>
#include <source_mapping/Source_Point.h>
#include <underworld/schema/Dungeon.h>
#include "Parser_Lookup.h"

namespace imp_summoning {

  class Zookeeper {
      source_mapping::Source_File_Storage source_files;
      runic::Lexer_Lookup lexer_lookup;
      const runic_imp::Symbols lexicon;
      imp_summoning::Parser_Lookup lookup;

  public:
      Zookeeper();
      void load_file(const std::string &path, underworld::Dungeon &underworld_root);

      const runic_imp::Symbols &get_lexicon() {
        return lexicon;
      }

      Parser_Lookup &get_parser_lookup() {
        return lookup;
      }

      runic::Lexer_Lookup &get_lexer_lookup() {
        return lexer_lookup;
      }
  };
}