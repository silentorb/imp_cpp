#pragma once

#include <runic/common/Stream.h>
#include <runic_cpp/Cpp_Lexicon.h>
#include "Dungeon_Map.h"
#include "Context.h"

namespace cpp_scanning {

  class Header_Parser {
      Dungeon_Map &dungeons;
      runic::Stream &input;
      const runic_cpp::Symbols &lexicon;

      void skip_line();

      void process_namespace_member(Context &context);
      void process_namespace(Context &context);

  public:
      Header_Parser(Dungeon_Map &dungeons, runic::Stream &input, const runic_cpp::Symbols &lexicon) :
        dungeons(dungeons), input(input), lexicon(lexicon) {}

      void parse();
  };
}