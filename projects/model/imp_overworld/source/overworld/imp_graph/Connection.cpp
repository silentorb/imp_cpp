#include <overworld/schema/Dungeon.h>
#include <overworld/schema/Dungeon_Reference.h>
#include <overworld/schema/Parameter.h>
#include <overworld/schema/Function_Signature.h>
#include <overworld/expressions/Member_Expression.h>

namespace overworld {

  Profession_Reference Connection::get_profession(Node &node) {
    return node.get_profession();
  }

  Profession_Reference Variant_To_Argument::get_profession(Node &node) {
    auto &first = get_first();
    auto &second = get_second();

    if (&node == &get_first()) {
      auto &compound_profession = first.get_profession();
      auto &dungeon = compound_profession->get_dungeon_interface();
      auto &arguments = dungeon.get_arguments();
      return arguments[parameter_index]->get_node().get_profession();
    }
    else {
      auto &compound_profession = first.get_profession();
      auto &dungeon = compound_profession->get_dungeon_interface();
      std::vector<overworld::Profession_Reference> professions;
      auto argument_profession = second.get_profession();
      argument_profession.set_ownership(dungeon.get_arguments()[0]->get_profession().get_ownership());
      professions.push_back(argument_profession);
      auto new_variant = new Dungeon(dungeon, professions);
      auto dungeon_reference = new Dungeon_Reference(Dungeon_Owner(new_variant));
      return Profession_Reference(dungeon_reference, dungeon.get_ownership());
    }
  }

  Profession_Reference derive_function_signature(Function_Signature &old_signature,
                                                 Profession_Reference &new_profession, int index) {
    auto signature = new overworld::Function_Signature();
    auto result = overworld::Profession_Reference(signature);
    auto &old_elements = old_signature.get_elements();
    auto &first_old_element = old_elements[0];

    for (auto i = 0; i < old_elements.size(); ++i) {
      if (i == index) {
        signature->add_element(overworld::Parameter_Owner(new overworld::Parameter(
          first_old_element->get_name(),
          new_profession,
          first_old_element->get_element().get_parent(),
          first_old_element->get_element().get_source_point()
        )));
      }
      else {
        auto &element = old_elements[i];
        signature->add_element(overworld::Parameter_Owner(new overworld::Parameter(
          element->get_name(),
          element->get_profession(),
          element->get_element().get_parent(),
          element->get_element().get_source_point()
        )));
      }
    }

    return result;
  }

  Profession_Reference Variant_To_Lambda::get_profession(Node &node) {
    auto &first = get_first();
    auto &second = get_second();

    auto &compound_profession = first.get_profession();
    auto &dungeon = compound_profession->get_dungeon_interface();
    auto &lambda_profession = second.get_profession().get_base(second.get_profession());
    auto signature = static_cast<Function_Signature *>(lambda_profession.get());

    if (&node == &get_first()) {
      auto variant = static_cast<Dungeon *>(&dungeon);
      auto &arguments = variant->get_arguments();
      auto &new_profession = arguments[variant_parameter_index]->get_node().get_profession();
      return derive_function_signature(*signature, new_profession, lambda_parameter_index);
    }
    else {

//      auto variant = static_cast<Dungeon *>(&dungeon);
      std::vector<overworld::Profession_Reference> professions;
      professions.push_back(signature->get_elements()[lambda_parameter_index]->get_profession());
      auto new_variant = new Dungeon(dungeon, professions);
      auto dungeon_reference = new Dungeon_Reference(Dungeon_Owner(new_variant));
      return Profession_Reference(dungeon_reference);
    }
  }

  Profession_Reference Lambda_To_Parameter::get_profession(Node &node) {
    auto &first = get_first();
    auto &second = get_second();

    auto &parameter_profession = second.get_profession().get_base(second.get_profession());
    if (&node == &get_first()) {
      auto &lambda_profession = first.get_profession().get_base(first.get_profession());
      auto signature = static_cast<Function_Signature *>(lambda_profession.get());
      return signature->get_elements()[lambda_parameter_index]->get_profession();
//      throw std::runtime_error("Not implemented");
    }
    else {
      return parameter_profession;
    }
  }
/*
  Profession_Reference Container_To_Member::get_profession(Node &node) {
    auto &first = get_first();
    auto &second = get_second();

    if (&node == &get_first()) {
      auto &first_profession = first.get_profession();
      auto &dungeon = first_profession->get_dungeon_interface();
      auto dungeon_member = dungeon.get_scope().get_member_or_null(member_name);

      // There should eventually be a mechanism for this to return a conflict.
      if (!dungeon_member)
        throw std::runtime_error("Could not find member named " + member_name);

      auto expression_element = dynamic_cast<Expression_Element *>(&second.get_element());
      auto member_expression = dynamic_cast<Member_Expression *>(&expression_element->get_expression());
      auto &member = member_expression->get_member();
      if (member.get_type() == Member_Type::temporary) {
        throw std::runtime_error("Not implemented.");
      }
      return overworld::get_member_profession_reference(*dungeon_member);
    }
    else {
      auto second_profession = second.get_profession();
      throw std::runtime_error("Not implemented.");
    }

  }
  */
}