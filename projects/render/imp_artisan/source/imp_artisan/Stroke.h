#pragma once

#include <vector>
#include <memory>

namespace imp_artisan {

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

  using Stroke_Owner = std::unique_ptr<Stroke>;
  using Strokes = std::vector<Stroke_Owner>;

  class Text : public virtual Stroke {
      const std::string value;

  public:
      Text(const std::string &value) : value(value) {}

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
      Strokes strokes;

  public:
      void add(Stroke *stroke) {
        strokes.push_back(Stroke_Owner(stroke));
      }

      void add(const std::string &text) {
        add(new Text(text));
      }

      const Strokes &get_strokes() const {
        return strokes;
      }

      Stroke_Stream &operator<<(const std::string &text) {
        add(new Text(text));
        return *this;
      }

      Stroke_Stream &operator<<(Stroke *stroke) {
        add(stroke);
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

      void add_stroke(Stroke *stroke) {
        strokes.push_back(Stroke_Owner(stroke));
      }

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

      void add_stroke(Stroke *stroke) {
        strokes.push_back(Stroke_Owner(stroke));
      }

      Type get_type() const override {
        return Type::group;
      }
  };
}