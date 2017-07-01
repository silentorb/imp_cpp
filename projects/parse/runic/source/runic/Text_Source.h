#pragma once

namespace runic {

  template<typename Character>
  class Text_Source {
  public:
      virtual Character next_character() = 0;
      virtual ~Text_Source() {}
  };
}