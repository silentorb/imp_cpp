#include "Profession_Library.h"
#include "cloning.h"
#include <overworld/schema/Function.h>
#include <overworld/schema/Dungeon_Reference.h>
#include <overworld/schema/Variant.h>

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

  Profession_Library::Profession_Library() {

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

//  Profession_Reference clone_profession(Profession_Reference &profession) {
//    switch (profession.get_ownership()) {
//
//      case Ownership::value: {
//        auto result = profession;
//        result.set_ownership(Ownership::reference);
//        return result;
//      }
//
//      default:
//        return profession;
//    }
//  }

//  Ownership get_assigned_ownership(Profession_Reference &source, Profession_Reference &target) {
//    if (target.get_ownership() == Ownership::unknown) {
//      if (source.get_ownership() == Ownership::value)
//        return Ownership::reference;
//
//      return source.get_ownership();
//    }
//    else {
//      return target.get_ownership();
//    }
//  }

  void Profession_Library::assign(Node &node, Profession_Reference &profession, Profession_Setter &setter) {
    auto &previous = node.get_profession();
//    if (previous.get_type() == Profession_Type::reference) {
//      auto reference = dynamic_cast<Reference *>(previous.get());
      auto base_source = profession.get_base(profession);
      auto new_profession = Profession_Reference(base_source);
      node.set_profession(new_profession, setter);
//    }
//    else {
//      node.set_profession(profession, setter);
//    }
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

  Function_Array &Profession_Library::get_function_variant_array(Function &function) {
    return get_or_create_variant_map(Functions, function);
  }

  Dungeon_Variant_Array &Profession_Library::get_dungeon_variant_array(Dungeon &dungeon) {
    return get_or_create_variant_map(dungeon_variants, dungeon);
  }

  Function &Profession_Library::create_function_variant(Function_Array &variant_array,
                                                        Function &function,
                                                        Dungeon &dungeon,
                                                        std::vector<Profession_Reference> &professions) {

    auto variant = new Virtual_Function(function);
    variant_array.push_back(Function_Owner(variant));
    throw std::runtime_error("Outdated, needs work handling generic arguments.");
//    for (auto i = 0; i < generic_parameters.size(); ++i) {
//      arguments.push_back(Generic_Argument_Owner(new Generic_Argument(*generic_parameters[i], professions[i])));
//    }
    return *variant;
  }

  template<typename A, typename B>
  A *find_variant(std::vector<B> &variant_array, std::vector<Profession_Reference> &professions) {
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
        return &variant;

      next:;
    }

    return nullptr;
  }

  Profession_Reference *find_variant(Dungeon_Variant_Array &variant_array,
                                     std::vector<Profession_Reference> &professions) {
    for (auto &profession : professions) {
      if (profession->get_type() == Profession_Type::unknown)
        return nullptr;
    }

    for (auto &variant : variant_array) {
      auto dungeon_variant = dynamic_cast<Dungeon *>(variant.get());
      for (auto &argument : dungeon_variant->get_arguments()) {
        if (argument->get_node().get_profession().get_type() == Profession_Type::unknown)
          goto next;
      }

      if (professions_match(dungeon_variant->get_generic_parameters(), professions))
        return &variant;

      next:;
    }

    return nullptr;
  }

  Function *Profession_Library::get_function_variant(Function_Array &variant_array,
                                                     Function &function,
                                                     std::vector<Profession_Reference> &professions) {
    auto result = find_variant<Function_Owner>(variant_array, professions);
    return result
           ? result->get()
           : nullptr;
//    for (auto &variant : variant_array) {
//      if (professions_match(variant->get_generic_parameters(), professions))
//        return variant.get();
//    }
//
//    return nullptr;
  }

  Dungeon_Variant_Tuple Profession_Library::create_dungeon_variant(Dungeon_Variant_Array &variant_array,
                                                                   Dungeon &dungeon,
                                                                   std::vector<Profession_Reference> &professions) {
//    throw std::runtime_error("Outdated.");
    auto variant = new Dungeon(dungeon);
    auto dungeon_reference = new Dungeon_Reference(std::move(Dungeon_Owner(variant)));
    for (auto &profession : professions) {
      variant->add_generic_argument(profession, {});
    }
    auto reference = Profession_Reference(dungeon_reference);
    variant_array.push_back(reference);
    return Dungeon_Variant_Tuple{reference, *variant};
  }

  Profession_Reference *Profession_Library::get_dungeon_variant(Dungeon_Variant_Array &variant_array,
                                                                std::vector<Profession_Reference> &professions) {
    return find_variant(variant_array, professions);
//    for (auto &variant : variant_array) {
//      if (professions_match(variant->get_generic_parameters(), professions))
//        return variant.get();
//    }
//
//    return nullptr;
  }

  Profession_Reference Profession_Library::get_or_create_dungeon_variant(Dungeon &dungeon,
                                                                         std::vector<Profession_Reference> &professions,
                                                                         overworld::Graph &graph) {
    auto &variant_array = get_dungeon_variant_array(dungeon);
    auto variant = Profession_Library::get_dungeon_variant(variant_array, professions);
    if (variant) {
      return *variant;
    }
    else {
      auto result = Profession_Library::create_dungeon_variant(variant_array, dungeon, professions);
//      clone_dungeon_graph(std::get<1>(result), graph);
      return std::get<0>(result);
    }
  }

//  Dungeon &Profession_Library::create_dungeon_variant(overworld::Dungeon_Variant_Array &variant_array,
//                                                              overworld::Dungeon &dungeon,
//                                                              Node &starting_node,
//                                                              overworld::Profession_Reference &profession) {
////    auto professions = to_professions(dungeon.get_generic_parameters(), 1);
//    std::vector<Profession_Reference> professions;
//    professions.push_back(profession);
//    auto variant = Profession_Library::get_dungeon_variant(variant_array, professions);
//    if (!variant) {
//      variant = &Profession_Library::create_dungeon_variant(
//        variant_array, dungeon, professions
//      );
//      clone_dungeon_graph(*variant, graph);
//    }
//
//    return *variant;
//  }

//  void Profession_Library::store_profession(Profession_Owner profession) {
//    profession_store.push_back(std::move(profession));
//  }
//  Dungeon &Profession_Library::resolve_with_existing_template_function(Node &node, Profession &profession) {
//    auto &function = node.get_function()->get_original();
//    auto &dungeon = function.get_node().get_dungeon()->get_original();
//    auto &variant_array = get_dungeon_variant_array(dungeon);
//    return create_dungeon_variant(variant_array, dungeon, node, profession);
//
//  }

}