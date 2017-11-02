#pragma once

#include <vector>
#include <memory>

namespace imp_artisan {

  const std::string tab_string = "  ";
  using Indent = std::string;

  namespace building {
    class Stroke_Owner;
  }

  namespace internal {

    class Stroke {
    public:
//        enum class Type {
//            block,
//            group,
//            special,
//            text,
//        };

//        virtual Type get_type() const = 0;
        virtual bool is_paragraph() const = 0;
        virtual std::string render(const Indent &indent) const = 0;

        virtual ~Stroke() {

        }
    };

    class Stroke_Stream;
  }

  namespace building {

    class Stroke_Owner {
        std::unique_ptr<internal::Stroke> stroke;

    protected:
        internal::Stroke_Stream &get_stream();

    public:
        Stroke_Owner();

        Stroke_Owner(internal::Stroke *stroke) :
          stroke(stroke) {}

        Stroke_Owner(const std::string &value);

        Stroke_Owner(const Stroke_Owner &other) = delete;

        Stroke_Owner(Stroke_Owner &&other) :
          stroke(std::move(other.stroke)) {}

        virtual ~Stroke_Owner() {}

        Stroke_Owner &operator=(Stroke_Owner &&other) noexcept {
          stroke = std::move(other.stroke);
          return *this;
        }

        const internal::Stroke *get_pointer() const {
          return stroke.get();
        }

        Stroke_Owner &operator<<(const std::string &text);
        Stroke_Owner &operator<<(Stroke_Owner &other);
        Stroke_Owner &operator<<(Stroke_Owner other);

        inline void add(Stroke_Owner &other);

//        const std::string &get_value() const;

        bool is_paragraph() const {
          return stroke->is_paragraph();
        }

        const std::string render(const Indent &indent) const {
          return stroke->render(indent);
        }
    };

    using Strokes = std::vector<Stroke_Owner>;

  }

  namespace internal {

    class Text_Stroke : public virtual Stroke {
        const std::string value;

    public:
        Text_Stroke(const std::string &value) : value(value) {}

        virtual ~Text_Stroke() {

        }

        const std::string &get_value() const {
          return value;
        }

        bool is_paragraph() const override {
          return false;
        }

        std::string render(const Indent &indent) const override {
          return indent + value;
        }
    };

    class Stroke_Stream {
    protected:
        building::Strokes strokes;

    public:
        virtual ~Stroke_Stream() {

        }

        void add_stroke(Stroke *stroke) {
          strokes.push_back(building::Stroke_Owner(stroke));
        }

        void add_stroke(building::Stroke_Owner &stroke) {
          strokes.push_back(std::move(stroke));
        }

        void add_stroke_owner(building::Stroke_Owner stroke) {
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
    protected:
        std::string create_indent(int steps) const;
        const std::string render_main(const Indent &indent, int steps, const std::string &opening,
                                      const std::string &closing) const;

    public:
        bool is_paragraph() const override {
          return true;
        }

    };

    class Standard_Block : public Block {
    protected:
        std::string header;

    protected:
        const std::string render_standard(const Indent &indent, int steps) const;

    public:
        explicit Standard_Block(const std::string &header) :
          header(header) {}

        virtual ~Standard_Block() {}

        std::string render(const Indent &indent) const override;
    };

    class Group : public virtual Stroke, public Stroke_Stream {

    public:
        Group() {}

        Group(std::initializer_list<Stroke *> new_strokes) {
          for (auto stroke: new_strokes) {
            add_stroke(stroke);
          }
        }

        bool is_paragraph() const override {
          return true;
        }

        std::string render(const Indent &indent) const override;
    };

  }

  class Tight_Group : public internal::Group {

  public:
      Tight_Group() {}

      Tight_Group(std::initializer_list<internal::Stroke *> new_strokes) :
        Group(new_strokes) {}

      Tight_Group(std::initializer_list<building::Stroke_Owner> new_strokes) {
        for (auto &stroke: new_strokes) {
          add_stroke_owner(std::move(*const_cast<building::Stroke_Owner *>(&stroke)));
        }
      }

      std::string render(const Indent &indent) const override;
  };

  std::string render_strokes(const building::Strokes &strokes, const Indent &indent);

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

    void Stroke_Owner::add(Stroke_Owner &other) {
      get_stream() << other;
    }

    inline Stroke_Owner::Stroke_Owner(const std::string &value) :
      stroke(new internal::Text_Stroke(value)) {}

    inline internal::Stroke_Stream &Stroke_Owner::get_stream() {
      return *dynamic_cast<internal::Stroke_Stream *>(stroke.get());
    }
  }
}