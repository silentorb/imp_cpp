#include <overworld/schema/Function.h>
#include <overworld/exploring/Expression_Explorer.h>
#include <overworld/expressions/Member_Expression.h>
#include <overworld/schema/Dungeon_Reference.h>
#include "dependency_gathering.h"

using namespace overworld;

namespace imp_rendering {

  using Profession_Delegate = std::function<void(const Profession &)>;
  using Member_Delegate = std::function<void(const Member &)>;

  class Dependency_Explorer {
      const Member_Delegate member_action;

      void explore_expression(const Expression &expression) {
        if (expression.is_statement())
          return;

        if (expression.get_type() == Expression_Type::member) {
          auto &member = dynamic_cast<const Member_Expression *>(&expression)->get_member();
          if (member.get_type() == Member_Type::function)
            member_action(member);
          else
            return;
        }
        else if (expression.get_type() == Expression_Type::literal
                 || expression.get_type() == Expression_Type::chain) {
          return;
        }

        auto &profession = expression.get_profession();
        if (profession.get_type() != Profession_Type::unknown && profession.get_type() != Profession_Type::Void)
          member_action(Member(profession));
      }

  public:
      Dependency_Explorer(const Member_Delegate &member_action) :
        member_action(member_action) {}

      void explore_block(const Block &block) {
        overworld::exploring::Expression_Explorer explorer([this](const Expression &expression) {
          explore_expression(expression);
        });

        explorer.explore_block(block);
      }
  };

  bool has_file(std::vector<File_Reference> &files, const File &file) {
    for (auto item: files) {
      if (&item.get_file() == &file)
        return true;;
    }

    return false;
  }

  void Include_Manager::process_expression(const overworld::Expression &expression) {
    switch (expression.get_type()) {
//      case Expression::Type::return_with_value:
    }
  }

  enum class Reference_Type {
      partial,
      full
  };

  struct Member_Usage {
      Member member;
      Reference_Type degree;

      Member_Usage() {}

      Member_Usage(const Member &member, Reference_Type degree) : member(member), degree(degree) {}
  };

  using Usage_Map = std::map<const void *, Member_Usage>;

  bool needs_reference(const Profession &profession) {
    if (profession.get_type() == Profession_Type::dungeon)
      return true;

    if (&profession == &Profession_Library::get_string())
      return true;

    return false;
  }

  bool is_needed(const Profession &profession, const Dungeon &dungeon) {
    if (!needs_reference(profession))
      return false;

    if (profession.get_type() == Profession_Type::dungeon) {
      auto dungeon_reference = dynamic_cast<const Dungeon_Reference *>(&profession);
      if (&dungeon_reference->get_dungeon() == &dungeon)
        return false;
    }

    return true;
  }

  class Include_Helper {
      Usage_Map &references;
      const Dungeon &dungeon;

  public:
      Include_Helper(Usage_Map &references, const Dungeon &dungeon) :
        references(references), dungeon(dungeon) {}

      void add_full(const Member &member) {
        if (member.get_type() == Member_Type::profession) {
          auto &profession = member.get_profession();
          if (profession.get_type() == Profession_Type::variant) {
            auto &variant = dynamic_cast<const Dungeon_Variant &>(profession).get_original();
            add_full(Member(const_cast<Dungeon &>(variant)));
            return;
          }

          if (!is_needed(profession, dungeon))
            return;

          if (!references.count(&profession) || references[&profession].degree == Reference_Type::partial) {
            references[&profession] = {member, Reference_Type::full};
          }
        }
      }

      void add_partial(const Member &member) {
        if (member.get_type() == Member_Type::profession) {
          auto &profession = member.get_profession();

          if (!is_needed(profession, dungeon))
            return;

          if (!references.count(&profession)) {
            references[&profession] = {member, Reference_Type::partial};
          }
        }
      }
  };


  class Source_Helper {
      const Usage_Map &include_references;
      Usage_Map &source_references;
      const Dungeon &dungeon;

  public:
      Source_Helper(const Usage_Map &include_references, Usage_Map &source_references,
                    const Dungeon &dungeon) :
        include_references(include_references), source_references(source_references), dungeon(dungeon) {}

      void add_full(const Member &member) {
        if (member.get_type() == Member_Type::profession) {
          auto &profession = member.get_profession();
          if (include_references.count(&profession) &&
              include_references.at(&profession).degree == Reference_Type::full)
            return;

          if (!source_references.count(&profession) ||
              source_references[&profession].degree == Reference_Type::partial) {
            source_references[&profession] = {member, Reference_Type::full};
          }
        }
        else if (member.get_type() == Member_Type::dungeon) {
          auto &profession = member.get_dungeon();
          if (&profession == &dungeon)
            return;

          if (include_references.count(&profession) &&
              include_references.at(&profession).degree == Reference_Type::full)
            return;

          if (!source_references.count(&profession) ||
              source_references[&profession].degree == Reference_Type::partial) {
            source_references[&profession] = {member, Reference_Type::full};
          }
        }
        else if (member.get_type() == Member_Type::function) {
          source_references[member.get_value()] = {member, Reference_Type::full};
        }
      }

      void add_partial(const Member &member) {
        add_full(member);
      }
  };

  template<typename Helper>
  class Gatherer {
      const Dungeon &dungeon;
      cpp_stl::Standard_Library &standard_library;
      Helper &helper;

      void process_function_declaration(const Function &function) {
        for (auto &parameter: function.get_signature().get_elements()) {
          auto &profession = parameter->get_profession();
          if (profession.get_ownership() == Ownership::owner) {
            helper.add_full(standard_library.get_unique_pointer());
          }
          helper.add_partial(Member(*profession));
        }
      }

  public:
      Gatherer(const Dungeon &dungeon, cpp_stl::Standard_Library &standard_library, Helper &helper) :
        dungeon(dungeon), standard_library(standard_library), helper(helper) {}

      void process_profession(const overworld::Profession &profession) {

      }

      void process_member(const Member &member) {
        if (member.get_type() == Member_Type::profession) {
          auto &profession = member.get_profession();
          auto &base_profession = profession.get_base();
//        helper.add_full(profession);
          if (profession.get_ownership() == Ownership::owner) {
            helper.add_full(standard_library.get_unique_pointer());
          }
          if (profession.get_ownership() == Ownership::value) {
            helper.add_full(Member(base_profession));
          }
          else {
            helper.add_partial(Member(base_profession));
          }
        }
        else if (member.get_type() == Member_Type::function) {
          helper.add_full(member);
        }
      }

      void explore_block(const Block &block) {
        Dependency_Explorer explorer([this](const Member &member) {
          process_member(member);
        });

        explorer.explore_block(block);
      }

      void gather_header_references() {
        if (dungeon.get_base_dungeon()) {
          auto member = Member(*dungeon.get_base_dungeon());
          helper.add_full(member);
        }

        for (auto &function : dungeon.get_functions()) {
          process_function_declaration(*function);
          if (function->is_inline()) {
            auto with_block = static_cast<const overworld::Function_With_Block *>(function.get());
            if (with_block)
              explore_block(with_block->get_block());
          }
        }

        for (auto &minion: dungeon.get_minions()) {
          auto &profession = minion->get_profession();
          if (profession.get_ownership() == Ownership::owner) {
            helper.add_full(standard_library.get_unique_pointer());
          }
          if (profession.get_ownership() == Ownership::value) {
            helper.add_full(Member(*profession));
          }
          else {
            helper.add_partial(Member(*profession));
          }
        }
      }

      void gather_source_references() {
        for (auto &function : dungeon.get_functions()) {
          process_function_declaration(*function);
        }

        for (auto &function : dungeon.get_functions()) {
          process_function_declaration(*function);
          if (!function->is_inline()) {
            auto with_block = static_cast<const overworld::Function_With_Block *>(function.get());
            if (with_block)
              explore_block(with_block->get_block());
          }
        }
      }
  };

  void add_include(std::vector<overworld::File_Reference> &includes, const Profession &profession,
                   overworld::Profession_File_Map &header_file_map) {

    auto file = profession.get_file();
    if (!file)
      file = header_file_map.get_file(profession);

    if (file) {
      if (!has_file(includes, *file)) {
        profession.get_file();
        includes.push_back({*file, !file->is_external()});
      }
    }
  }

  void add_include(std::vector<overworld::File_Reference> &includes, const File *file,
                   overworld::Profession_File_Map &header_file_map) {
    if (file) {
      if (!has_file(includes, *file)) {
        includes.push_back({*file, !file->is_external()});
      }
    }
  }

  void add_include(std::vector<overworld::File_Reference> &includes, const Member member,
                   overworld::Profession_File_Map &header_file_map) {
    if (member.get_type() == Member_Type::profession) {
      auto &profession = member.get_profession();
      add_include(includes, profession, header_file_map);
    }
    else if (member.get_type() == Member_Type::function) {
      add_include(includes, member.get_function().get_file(), header_file_map);
    }
    else if (member.get_type() == Member_Type::dungeon) {
      add_include(includes, member.get_dungeon().get_file(), header_file_map);
    }
    else {
      throw std::runtime_error("Not implemented.");
    }
  }

  void Include_Manager::gather_headers(cpp_stl::Standard_Library &standard_library) {
    Usage_Map include_references;
    Usage_Map source_references;
    Include_Helper include_helper(include_references, dungeon);
    Source_Helper source_helper(include_references, source_references, dungeon);
    Gatherer<Include_Helper> include_gatherer(dungeon, standard_library, include_helper);
    Gatherer<Source_Helper> source_gatherer(dungeon, standard_library, source_helper);
    include_gatherer.gather_header_references();
    source_gatherer.gather_source_references();

    for (auto &entry : include_references) {
      auto &member = entry.second.member;
      if (entry.second.degree == Reference_Type::partial) {
        if (member.get_type() == Member_Type::profession) {
          forward_declarations.push_back(&member.get_profession());
        }
        else {
          throw std::runtime_error("Not implemented.");
        }
      }
      else {
        add_include(header_includes, member, header_file_map);
      }
    }

    for (auto &entry : source_references) {
      auto &member = entry.second.member;
      add_include(source_includes, member, header_file_map);
    }

    source_includes.push_back({*dungeon.get_file(), true});
  }

}