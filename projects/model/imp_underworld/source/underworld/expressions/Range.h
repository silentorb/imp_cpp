#pragma once

#include "Expression.h"

namespace underworld {

  enum class Range_Value_Type {
      none,
      absolute,
      all,
  };

  class Range_Value {
      long value;
      Range_Value_Type type;

  public:
      Range_Value() : value(0), type(Range_Value_Type::none) {}

      Range_Value(Range_Value_Type type, long value) : value(value), type(type) {}

      long get_value() const {
        return value;
      }

      Range_Value_Type get_type() const {
        return type;
      }

      const std::string to_string() const {
        switch (type) {
          case Range_Value_Type::none:
          case Range_Value_Type::all:
            return "";

          case Range_Value_Type::absolute:
            return std::to_string(value);
        }
      }
  };

  class Range : public Expression {
      Range_Value start;
      Range_Value end;

  public:
      Range(const Range_Value &start, const Range_Value &end = Range_Value()) :
        start(start), end(end) {}

      Expression_Type get_type() const override {
        return Expression_Type::range;
      }

      Expression &get_last() override {
        return *this;
      }

      const std::string get_name() const override {
        return "[" + start.to_string()
               + (end.get_type() == Range_Value_Type::none ? "" : ":" + end.to_string())
               + "]";
      }

      const Range_Value &get_start() const {
        return start;
      }

      const Range_Value &get_end() const {
        return end;
      }
  };
}