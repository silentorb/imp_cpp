#include "Profession_Library.h"
#include "cloning.h"
#include <overworld/schema/Function.h>

namespace overworld {

  File string_header("string", true);
  static Unknown unknown;
  static Void _void;
  static Profession_Reference unknown_reference(unknown);
  static Profession_Reference void_reference(_void);

//  static Unknown not_found;

  static Primitive primitives[] =
    {
      Primitive_Type::Bool,
      Primitive_Type::Double,
      Primitive_Type::Float,
      Primitive_Type::Int,
      {Primitive_Type::String, &string_header},
//      Primitive_Type::Void,
    };

  static Profession_Reference primitive_references[] =
    {
      Profession_Reference(primitives[0]),
      Profession_Reference(primitives[1]),
      Profession_Reference(primitives[2]),
      Profession_Reference(primitives[3]),
      Profession_Reference(primitives[4]),
    };

  Profession_Library::Profession_Library(Graph &graph) : graph(graph) {

  }

  Profession_Reference &Profession_Library::get_unknown() {
    return unknown_reference;
  }

  Profession_Reference &Profession_Library::get_void() {
    return void_reference;
  }

  Primitive &Profession_Library::get_string() {
    return primitives[static_cast<int>(Primitive_Type::String)];
  }

  File &Profession_Library::get_string_file() {
    return string_header;
  }

  Profession_Reference &Profession_Library::get_primitive(Primitive_Type type) {
    return primitive_references[static_cast<int>(type)];
  }

//  Profession_Reference &Profession_Library::get_reference(Profession &profession) {
//    if (references.count(&profession))
//      return *references[&profession];
//
//    auto reference = new Reference(profession);
//    references[&profession] = std::unique_ptr<Reference>(reference);
//    return *reference;
//  }

//  Pointer &Profession_Library::get_pointer(Profession &profession) {
//    if (pointers.count(&profession))
//      return *pointers[&profession];
//
//    auto pointer = new Pointer(profession);
//    pointers[&profession] = std::unique_ptr<Pointer>(pointer);
//    return *pointer;
//  }

  void Profession_Library::assign(Node &node, overworld::Profession_Reference &profession, Profession_Setter &setter) {
    auto &previous = node.get_profession();
    if (previous.get_type() == Profession_Type::reference) {
      auto reference = dynamic_cast<Reference *>(previous.get());
      auto new_profession = reference->is_pointer()
                            ? Profession_Reference(new Pointer(profession.get_base(profession)))
                            : Profession_Reference(new Reference(profession.get_base(profession)));
      node.set_profession(new_profession, setter);
    }
    else {
      node.set_profession(profession, setter);
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
                                                                std::vector<Profession_Reference> &professions) {

    auto variant = new Function_Variant(function.get_original(), dungeon, professions);
    variant_array.push_back(Function_Variant_Owner(variant));
    return *variant;
  }

  template<typename A>
  A *find_variant(std::vector<std::unique_ptr<A>> &variant_array, std::vector<Profession_Reference> &professions) {
    for (auto &profession : professions) {
      if (profession->get_type() == Profession_Type::unknown)
        return nullptr;
    }

    for (auto &variant : variant_array) {
      for (auto &argument : variant->get_arguments()) {
        if (argument->get_node().get_profession().get_type() == Profession_Type::unknown)
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
                                                             std::vector<Profession_Reference> &professions) {
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
                                                              std::vector<Profession_Reference> &professions) {
    auto variant = new Dungeon_Variant(dungeon.get_original(), professions);
    variant_array.push_back(Dungeon_Variant_Owner(variant));
    return *variant;
  }

  Dungeon_Variant *Profession_Library::get_dungeon_variant(Dungeon_Variant_Array &variant_array,
                                                           std::vector<Profession_Reference> &professions) {
    return find_variant(variant_array, professions);
//    for (auto &variant : variant_array) {
//      if (professions_match(variant->get_generic_parameters(), professions))
//        return variant.get();
//    }
//
//    return nullptr;
  }

  Dungeon_Variant &Profession_Library::get_or_create_dungeon_variant(Dungeon &dungeon,
                                                                     std::vector<Profession_Reference> &professions,
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
                                                              Node &starting_node, overworld::Profession_Reference &profession) {
//    auto professions = to_professions(dungeon.get_generic_parameters(), 1);
    std::vector<Profession_Reference> professions;
    professions.push_back(profession);
    auto variant = Profession_Library::get_dungeon_variant(variant_array, professions);
    if (!variant) {
      variant = &Profession_Library::create_dungeon_variant(
        variant_array, dungeon, professions
      );
      clone_dungeon_graph(*variant, graph);
    }

    return *variant;
  }

  void Profession_Library::store_profession(Profession_Owner profession) {
    profession_store.push_back(std::move(profession));
  }
//  Dungeon_Variant &Profession_Library::resolve_with_existing_template_function(Node &node, Profession &profession) {
//    auto &function = node.get_function()->get_original();
//    auto &dungeon = function.get_node().get_dungeon()->get_original();
//    auto &variant_array = get_dungeon_variant_array(dungeon);
//    return create_dungeon_variant(variant_array, dungeon, node, profession);
//
//  }

}