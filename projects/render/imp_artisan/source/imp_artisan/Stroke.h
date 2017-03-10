#pragma once

#include <vector>
#include <memory>

namespace imp_artisan {
  namespace internal {

    class Stroke {
    public:
        enum class Type {
            block,
            group,
            special,
            text,
        };

        virtual Type get_type() const = 0;
    };

    class Stroke_Stream;
  }

  namespace building {
//  using Stroke_Owner = std::unique_ptr<Stroke>;
    class Stroke_Owner {
        std::unique_ptr<internal::Stroke> stroke;

    protected:
        internal::Stroke_Stream &get_stream();

    public:
//        Stroke_Owner(Stroke_Owner &stroke) :
//          stroke(std::move(stroke)) {}
        Stroke_Owner();

        Stroke_Owner(internal::Stroke *stroke) :
          stroke(stroke) {}

        Stroke_Owner(const std::string &value);

        Stroke_Owner(const Stroke_Owner &other) = delete;

        Stroke_Owner(Stroke_Owner &&other) :
          stroke(std::move(other.stroke)) {}

        Stroke_Owner &operator=(Stroke_Owner &&other) noexcept {
          stroke = std::move(other.stroke);
          return *this;
        }

        internal::Stroke::Type get_type() const {
          return stroke->get_type();
        }

        const internal::Stroke *get_pointer() const {
          return stroke.get();
        }

        Stroke_Owner &operator<<(const std::string &text);
        Stroke_Owner &operator<<(Stroke_Owner &other);
        Stroke_Owner &operator<<(Stroke_Owner other);

        const std::string &get_value() const;
    };

    using Strokes = std::vector<Stroke_Owner>;

  }

  namespace internal {
    class Text_Stroke : public virtual Stroke {
        const std::string value;

    public:
        Text_Stroke(const std::string &value) : value(value) {}

        const std::string &get_value() const {
          return value;
        }

        Type get_type() const override {
          return Type::text;
        }
    };

    class Special_Text : public virtual Stroke {
        const std::string value;

    public:
        Special_Text(const std::string &value) : value(value) {}

        const std::string &get_value() const {
          return value;
        }

        Type get_type() const override {
          return Type::special;
        }
    };

    class Stroke_Stream {
    protected:
        building::Strokes strokes;

    public:
        void add_stroke(Stroke *stroke) {
          strokes.push_back(building::Stroke_Owner(stroke));
        }

        void add_stroke(building::Stroke_Owner &stroke) {
          strokes.push_back(std::move(stroke));
        }

        void add_stroke(const std::string &text) {
          add_stroke(new Text_Stroke(text));
        }

        const building::Strokes &get_strokes() const {
          return strokes;
        }

        Stroke_Stream &operator<<(const std::string &text) {
          add_stroke(new Text_Stroke(text));
          return *this;
        }

        Stroke_Stream &operator<<(Stroke *stroke) {
          add_stroke(stroke);
          return *this;
        }

        Stroke_Stream &operator<<(building::Stroke_Owner &stroke) {
          add_stroke(stroke);
          return *this;
        }
    };

    class Block : public virtual Stroke, public Stroke_Stream {
        std::string header;
        std::string footer;
        int indent = 1;

    public:
        Block(const std::string &header, const std::string &footer, int indent = 1) :
          header(header), footer(footer), indent(indent) {}

        const std::string &get_header() const {
          return header;
        }

        const std::string &get_footer() const {
          return footer;
        }

        Type get_type() const override {
          return Type::block;
        }

        int get_indent() const {
          return indent;
        }
    };

    class Group : public virtual Stroke, public Stroke_Stream {

    public:
        Group() {}

        Group(std::initializer_list<Stroke *> new_strokes) {
          for (auto stroke: new_strokes) {
            add_stroke(stroke);
          }
        }

        Type get_type() const override {
          return Type::group;
        }
    };
  }

  namespace building {
    inline Stroke_Owner::Stroke_Owner() : stroke(new internal::Group()) {}

    inline Stroke_Owner &Stroke_Owner::operator<<(const std::string &text) {
      get_stream() << text;
      return *this;
    }

    inline Stroke_Owner &Stroke_Owner::operator<<(Stroke_Owner &other) {
      get_stream() << other;
      return *this;
    }

    inline Stroke_Owner &Stroke_Owner::operator<<(Stroke_Owner other) {
      get_stream() << other;
      return *this;
    }

    inline Stroke_Owner::Stroke_Owner(const std::string &value) :
      stroke(new internal::Text_Stroke(value)) {}

     inline internal::Stroke_Stream &Stroke_Owner::get_stream() {
      if (stroke->get_type() == internal::Stroke::Type::group) {
        return *dynamic_cast<internal::Group *>(stroke.get());
      }
      else {
        return *dynamic_cast<internal::Block *>(stroke.get());
      }
    }

    inline const std::string &Stroke_Owner::get_value() const {
      if (stroke->get_type() == internal::Stroke::Type::text) {
        return dynamic_cast<const internal::Text_Stroke *>(stroke.get())->get_value();
      }
      else {
        return dynamic_cast<const internal::Special_Text *>(stroke.get())->get_value();
      }
    }

  }
}