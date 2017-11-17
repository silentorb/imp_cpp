#pragma once

#include <overworld/expressions/Member_Expression.h>
#include <overworld/imp_graph/Graph.h>

namespace overworld {

  class Temporary_Member {
      Common_Element element;
      Node node;
      std::vector<Member_Expression *> expressions;
//      std::unique_ptr<Function_Signature> signature;

  public:
      Temporary_Member(const std::string &name, Profession_Reference &profession,
                       const source_mapping::Source_Range source_point, Function &function) :
        element(Element_Type::temporary, name, Parent(), source_point),
        node(profession, element) {}

      virtual ~Temporary_Member() {}

      void add_expression(Member_Expression &expression) {
        expressions.push_back(&expression);
      }

      void replace_with(Member &member) {
        for (auto expression: expressions) {
          expression->set_member(member);
        }
      }

      const std::string get_name() const {
        return element.get_name();
      }

      Node &get_node() {
        return node;
      }

//      Function_Signature &get_or_create_signature() {
//        if (!signature) {
//          signature = std::unique_ptr<Function_Signature>(new Function_Signature());
////          signature->add_element(Simple_Parameter_Owner(new Simple_Parameter(
////            Profession_Library::get_unknown(),
////            nullptr, nullptr, source_mapping::Source_Range()
////          )));
//        }
//
//        return *signature;
//      }

//      void add_parameter(Parameter_Owner parameter) {
//        signature->add_element(std::move(parameter));
//      }
  };

  class Temporary_Interface : public Profession {
      std::vector<std::unique_ptr<Temporary_Member>> members;

  public:
		virtual ~Temporary_Interface() {	}

      void add_member(std::unique_ptr<Temporary_Member> member) {
        members.push_back(std::move(member));
      }

      Profession_Type get_type() const override {
        return Profession_Type::temporary_interface;
      }

      const std::string get_name() const override {
        return "Temporary_Interface";
      }

      Ownership get_ownership() const override {
        return Ownership::unknown;
      }

      Expression_Owner create_temporary_member(Member_Expression &previous_expression,
                                               const std::string &child_name,
                                               const source_mapping::Source_Range source_point,
                                               overworld::Graph &graph,
                                               Scope &scope);

      void replace(Profession_Reference &profession);
  };

}