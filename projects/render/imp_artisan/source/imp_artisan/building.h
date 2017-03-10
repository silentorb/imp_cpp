#pragma once

#include "Stroke.h"

namespace imp_artisan {
  namespace building {

    class Stroke_Owner_Stream : public Stroke_Owner {
    public:
        Stroke_Owner_Stream(internal::Stroke *stroke) : Stroke_Owner(stroke) {}

    };

    class Block : public Stroke_Owner_Stream {
    public:
        Block(const std::string &header, const std::string &footer, int indent = 1)
          : Stroke_Owner_Stream(new internal::Block(header, footer, indent)) {}

    };
  }
}