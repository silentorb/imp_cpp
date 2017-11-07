#pragma once

#include <imp_artisan/Stroke.h>
#include <overworld/schema/Dungeon.h>
#include <overworld/expressions/Block_Expression.h>
#include <functional>
#include <overworld/schema/Function.h>

namespace cpp_stl {
  class Standard_Library;
}

namespace imp_rendering {

  void set_standard_library(cpp_stl::Standard_Library &library);

  using Stroke = imp_artisan::building::Stroke_Owner;
  const std::string render_function_parameters(const overworld::Function &function);
  const std::string render_function_return_signature(const overworld::Function &function);
  const std::string render_function_declaration(const overworld::Function &function);
  const std::string render_profession(const overworld::Profession_Reference &profession, const overworld::Scope &scope);
  const std::string render_profession_owner(const overworld::Profession_Reference &profession, const overworld::Scope &scope);
  const std::string render_separator(const overworld::Expression &expression);
  const std::string render_cast(const overworld::Profession &target, const overworld::Profession &source,
                                const std::string &value);
//  const std::string render_profession_as_owner_reference(const overworld::Profession &profession);

  const std::string get_cpp_name(const overworld::Dungeon &dungeon);
  const std::string get_cpp_name(const overworld::Minion &minion);
  const std::string get_cpp_name(const overworld::Parameter &parameter);
  const std::string get_cpp_name(const overworld::Function &function);

  const std::string render_namespace(const overworld::Parent &parent, const overworld::Parent *current, const std::string &delimiter);

  const std::string render_argument(const overworld::Expression &argument, const overworld::Parameter &parameter,
                                    const overworld::Scope &scope);
  const std::string render_expression(const overworld::Expression &input_expression, const overworld::Scope &scope);
  void render_statements(Stroke &stroke, const overworld::Expressions &statements, const overworld::Scope &scope);
  Stroke render_block(const std::string &header, const overworld::Block &block);
  Stroke render_shrinking_block(const std::string &header, const overworld::Block_Expression &block);
  Stroke render_statement(const overworld::Expression &input_expression, const overworld::Scope &scope);

  std::string sanitize_name(const std::string &name);

  Stroke render_function_definition(const overworld::Function &function);
  Stroke render_possible_namespace_block(const overworld::Parent &parent, Stroke stroke);
  Stroke render_possible_generic_block(const std::vector<overworld::Generic_Parameter *> &generic_parameters,
                                       Stroke stroke);
  const std::string render_minion_with_signature(const overworld::Minion &minion, const overworld::Scope &scope);
  const std::string render_parameter(const overworld::Minion &minion, const overworld::Scope &scope);
  Stroke render_includes(const std::vector<overworld::File_Reference> &files);

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