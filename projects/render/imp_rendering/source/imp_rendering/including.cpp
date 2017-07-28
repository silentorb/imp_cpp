#include <overworld/schema/Function.h>
#include "including.h"

using namespace overworld;

namespace imp_rendering {

  bool has_file(std::vector<File *> &files, File &file) {
    for (auto item: files) {
      if (item == &file)
        return true;;
    }

    return false;
  }

//  void Include_Manager::prepare(std::vector<const Profession *> dependencies, Profession_File_Map &file_map) {
//    for (auto dependency: dependencies) {
//      auto &profession = *dependency;
//
//      auto file = profession.get_file();
//      if (!file)
//        file = file_map.get_file(profession);
//
//      if (file) {
//        if (!has_file(source_includes, *file)) {
//          profession.get_file();
//          source_includes.push_back(file);
//        }
//      }
//    }
//
//    source_includes.push_back(file_map.get_file(dungeon));
//  }

  void Include_Manager::process_expression(const overworld::Expression &expression) {
    switch (expression.get_type()) {
//      case Expression::Type::return_with_value:
    }
  }

  void Include_Manager::gather_include_headers() {
    exploring::Expression_Explorer explorer([this](const Expression &expression) {
      process_expression(expression);
    });

    for (auto &function :  dungeon.get_functions()) {
      if (function->is_inline()) {
        explorer.explore_block(function->get_block());
      }
    }

  }

  enum class Reference_Type {
      partial,
      full
  };

  using Reference_Strength_Map = std::map<const overworld::Profession *, Reference_Type>;

  class Include_Helper {
      Reference_Strength_Map &references;
      const Dungeon &dungeon;

  public:
      Include_Helper(Reference_Strength_Map &references, const Dungeon &dungeon) :
        references(references), dungeon(dungeon) {}

      void add_full(const Profession &profession) {
        if (&profession == &dungeon)
          return;

        if (!references.count(&profession) || references[&profession] == Reference_Type::partial) {
          references[&profession] = Reference_Type::full;
        }
      }

      void add_partial(const Profession &profession) {
        if (&profession == &dungeon)
          return;

        if (!references.count(&profession)) {
          references[&profession] = Reference_Type::partial;
        }
      }
  };


  class Source_Helper {
      const Reference_Strength_Map &include_references;
      Reference_Strength_Map &source_references;
      const Dungeon &dungeon;

  public:
      Source_Helper(const Reference_Strength_Map &include_references, Reference_Strength_Map &source_references,
                    const Dungeon &dungeon) :
        include_references(include_references), source_references(source_references), dungeon(dungeon) {}

      void add_full(const Profession &profession) {
        if (&profession == &dungeon)
          return;

        if (include_references.count(&profession) && include_references.at(&profession) == Reference_Type::full)
          return;

        if (!source_references.count(&profession) || source_references[&profession] == Reference_Type::partial) {
          source_references[&profession] = Reference_Type::full;
        }
      }

      void add_partial(const Profession &profession) {
        add_full(profession);
      }
  };

  template<typename Helper>
  class Gatherer {
      const Dungeon &dungeon;
      cpp_stl::Standard_Library &standard_library;
      Helper &helper;

      void process_function_declaration(const Function &function) {
        for (auto &parameter: function.get_parameters()) {
          auto &profession = parameter->get_profession();
          if (profession.get_ownership() == Ownership::owner) {
            helper.add_full(standard_library.get_unique_pointer());
          }
          helper.add_partial(profession);
        }
      }

  public:
      Gatherer(const Dungeon &dungeon, cpp_stl::Standard_Library &standard_library, Helper &helper) :
        dungeon(dungeon), standard_library(standard_library), helper(helper) {}

      void gather_header_references() {
        for (auto &function : dungeon.get_functions()) {
          process_function_declaration(*function);
//      if (function->is_inline()) {
//        explorer.explore_block(function->get_block());
//      }
        }

        for (auto &minion: dungeon.get_minions()) {
          auto &profession = minion->get_profession();
          if (profession.get_ownership() == Ownership::value) {
            helper.add_full(standard_library.get_unique_pointer());
          }
        }
      }

      void gather_source_references() {
        for (auto &function : dungeon.get_functions()) {
          process_function_declaration(*function);
        }

//        helper.add_full(dungeon);
      }
  };

  void add_include(std::vector<overworld::File *> &includes, const Profession &profession,
                   overworld::Profession_File_Map &header_file_map) {

    auto file = profession.get_file();
    if (!file)
      file = header_file_map.get_file(profession);

    if (file) {
      if (!has_file(includes, *file)) {
        profession.get_file();
        includes.push_back(file);
      }
    }
  }

  void Include_Manager::gather_headers(cpp_stl::Standard_Library &standard_library) {
    Reference_Strength_Map include_references;
    Reference_Strength_Map source_references;
    Include_Helper include_helper(include_references, dungeon);
    Source_Helper source_helper(include_references, source_references, dungeon);
    Gatherer<Include_Helper> include_gatherer(dungeon, standard_library, include_helper);
    Gatherer<Source_Helper> source_gatherer(dungeon, standard_library, source_helper);
    include_gatherer.gather_header_references();
    source_gatherer.gather_source_references();

    for (auto &entry : include_references) {
      auto &profession = *entry.first;
      if (entry.second == Reference_Type::full) {
        add_include(header_includes, profession, header_file_map);
      }
      else {
        forward_declarations.push_back(&profession);
      }
    }

    for (auto &entry : source_references) {
      add_include(source_includes, *entry.first, header_file_map);
    }

    source_includes.push_back(dungeon.get_file());
  }

}