#pragma once

#include <imp_artisan/Stroke.h>
#include <overworld/schema/Dungeon.h>
#include <overworld/expressions/Block_Expression.h>
#include <functional>

namespace imp_rendering {

  using Stroke = imp_artisan::building::Stroke_Owner;
  const std::string render_function_parameters(const overworld::Function &function);
  const std::string render_function_return_signature(const overworld::Function &function);
  const std::string render_function_declaration(const overworld::Function &function);
  const std::string render_profession(const overworld::Profession &profession);
  const std::string render_profession_as_owner(const overworld::Profession &profession);
//  const std::string render_profession_as_owner_reference(const overworld::Profession &profession);

  const std::string render_expression(const overworld::Expression &input_expression);
  void render_statements(Stroke &stroke, const overworld::Expressions &statements, const overworld::Scope &scope);
  Stroke render_block(const std::string &header, const overworld::Block &block);
  Stroke render_shrinking_block(const std::string &header, const overworld::Block_Expression &block);
  Stroke render_statement(const overworld::Expression &input_expression, const overworld::Scope &scope);
  Stroke render_function_definition(const overworld::Function &function);
  const std::string render_minion_with_signature(const overworld::Minion &minion);
  const std::string render_parameter(const overworld::Minion &minion);
  Stroke render_includes(const std::vector<overworld::File *> &files);

  inline Stroke wrap(const std::string &text) {
    Stroke result;
    result << text;
    return result;
  }

  template<typename T>
  using Joiner = std::function<const std::string(const T &)>;

  template<typename T>
  const std::string join(const std::vector<T> &items, const Joiner<T> &converter,
                         const std::string delimiter) {
    if (items.size() == 0)
      return "";

    std::string result = converter(items[0]);
    for (int i = 1; i < items.size(); ++i) {
      result += delimiter + converter(items[i]);
    }

    return result;
  }


  template<typename Key, typename Value>
  using Map_Joiner = std::function<const std::string(const Key &, const Value &)>;

  template<typename Key, typename Value>
  const std::string join(const std::map<Key, Value> &items, const Map_Joiner<Key, Value> &converter,
                         const std::string delimiter) {
    if (items.size() == 0)
      return "";

    int i = 0;
    std::string result;
    for (auto &item : items) {
      if (i++ != 0)
        result += delimiter;

      result += converter(item.first, item.second);
    }

    return result;
  }
}