#include "Profession_Library.h"
#include "cloning.h"
#include <overworld/schema/Function.h>

namespace overworld {

  File string_header("", "string");
  static Unknown unknown;
  static Void _void;
//  static Unknown not_found;

  Primitive Profession_Library::primitives[] =
    {
      Primitive_Type::Bool,
      Primitive_Type::Double,
      Primitive_Type::Float,
      Primitive_Type::Int,
      {Primitive_Type::String, &string_header},
//      Primitive_Type::Void,
    };

  Profession_Library::Profession_Library(Graph &graph) : graph(graph) {

  }

  Unknown &Profession_Library::get_unknown() {
    return unknown;
  }

  Void &Profession_Library::get_void() {
    return _void;
  }

//  Unknown &Profession_Library::get_not_found() {
//    return not_found;
//  }

  Primitive &Profession_Library::get_primitive(Primitive_Type type) {
    return primitives[static_cast<int>(type)];
  }

  Reference &Profession_Library::get_reference(Profession &profession) {
    if (references.count(&profession))
      return *references[&profession];

    auto reference = new Reference(profession);
    references[&profession] = std::unique_ptr<Reference>(reference);
    return *reference;
  }

  Pointer &Profession_Library::get_pointer(Profession &profession) {
    if (pointers.count(&profession))
      return *pointers[&profession];

    auto pointer = new Pointer(profession);
    pointers[&profession] = std::unique_ptr<Pointer>(pointer);
    return *pointer;
  }

  void Profession_Library::assign(Node &node, overworld::Profession &profession) {
    auto &previous = node.get_profession();
    if (previous.get_type() == Profession_Type::reference) {
      auto &reference = *dynamic_cast<Reference *>(&previous);
      auto &new_profession = reference.is_pointer()
                             ? (Reference &) get_pointer(profession.get_base())
                             : get_reference(profession.get_base());
      node.set_profession(new_profession);
    }
    else {
      node.set_profession(profession);
    }
  }

  template<typename A, typename B>
  B &get_or_create_variant_map(std::unordered_map<A *, B> &generic_map, A &item) {
    if (!generic_map.count(&item)) {
      return generic_map.emplace(&item, B()).first->second;
    }
    else {
      return generic_map.at(&item);
    }
  };

  Function_Variant_Array &Profession_Library::get_function_variant_array(Function_Interface &function) {
    return get_or_create_variant_map(function_variants, function);
  }

  Dungeon_Variant_Array &Profession_Library::get_dungeon_variant_array(Dungeon_Interface &dungeon) {
    return get_or_create_variant_map(dungeon_variants, dungeon);
  }

  Function_Variant &Profession_Library::create_function_variant(Function_Variant_Array &variant_array,
                                                                Function_Interface &function,
                                                                Dungeon_Interface &dungeon,
                                                                std::vector<Profession *> &professions) {

    auto variant = new Function_Variant(function.get_original(), dungeon, professions);
    variant_array.push_back(Function_Variant_Owner(variant));
    return *variant;
  }

  template<typename A>
  A *find_variant(std::vector<std::unique_ptr<A>> &variant_array, std::vector<Profession *> &professions) {
    for (auto &profession : professions) {
      if (profession->get_type() == Profession_Type::unknown)
        return nullptr;
    }

    for (auto &variant : variant_array) {
      for (auto &profession : variant->get_professions()) {
        if (profession->get_type() == Profession_Type::unknown)
          goto next;
      }

      if (professions_match(variant->get_generic_parameters(), professions))
        return variant.get();

      next:;
    }

    return nullptr;
  }

  Function_Variant *Profession_Library::get_function_variant(Function_Variant_Array &variant_array,
                                                             Function_Interface &function,
                                                             std::vector<Profession *> &professions) {
    return find_variant(variant_array, professions);
//    for (auto &variant : variant_array) {
//      if (professions_match(variant->get_generic_parameters(), professions))
//        return variant.get();
//    }
//
//    return nullptr;
  }

  Dungeon_Variant &Profession_Library::create_dungeon_variant(Dungeon_Variant_Array &variant_array,
                                                              Dungeon_Interface &dungeon,
                                                              std::vector<Profession *> &professions) {
    auto variant = new Dungeon_Variant(dungeon.get_original(), professions);
    variant_array.push_back(Dungeon_Variant_Owner(variant));
    return *variant;
  }

  Dungeon_Variant *Profession_Library::get_dungeon_variant(Dungeon_Variant_Array &variant_array,
                                                           std::vector<Profession *> &professions) {
    return find_variant(variant_array, professions);
//    for (auto &variant : variant_array) {
//      if (professions_match(variant->get_generic_parameters(), professions))
//        return variant.get();
//    }
//
//    return nullptr;
  }

  Dungeon_Variant &Profession_Library::get_or_create_dungeon_variant(Dungeon &dungeon,
                                                                     std::vector<Profession *> &professions,
                                                                     overworld::Graph &graph) {
    auto &variant_array = get_dungeon_variant_array(dungeon);
    auto variant = Profession_Library::get_dungeon_variant(variant_array, professions);
    if (!variant) {
      variant = &Profession_Library::create_dungeon_variant(variant_array, dungeon, professions);
      clone_dungeon_graph(*variant, graph);
    }
    return *variant;
  }

  Dungeon_Variant &Profession_Library::create_dungeon_variant(overworld::Dungeon_Variant_Array &variant_array,
                                                              overworld::Dungeon &dungeon,
                                                              Node &starting_node, overworld::Profession &profession) {
//    auto professions = to_professions(dungeon.get_generic_parameters(), 1);
    std::vector<Profession *> professions;
    professions.push_back(&profession);
    auto variant = Profession_Library::get_dungeon_variant(variant_array, professions);
    if (!variant) {
      variant = &Profession_Library::create_dungeon_variant(
        variant_array, dungeon, professions
      );
      clone_dungeon_graph(*variant, graph);
    }

    return *variant;
  }

//  Dungeon_Variant &Profession_Library::resolve_with_existing_template_function(Node &node, Profession &profession) {
//    auto &function = node.get_function()->get_original();
//    auto &dungeon = function.get_node().get_dungeon()->get_original();
//    auto &variant_array = get_dungeon_variant_array(dungeon);
//    return create_dungeon_variant(variant_array, dungeon, node, profession);
//
//  }

}