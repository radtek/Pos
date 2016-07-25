#pragma hdrstop

#include "Predicates.h"

#pragma package(smart_init)

#include "MM_DBCore.h"

namespace SqlPredicates {

bool
TSimpleExistencePredicate::operator()()
const
{
   bool all_present = true;
   std::set<UnicodeString> present_items;

   retrieve_present_items(present_items);
   if (!items_to_check_for_.size() || !present_items.size())
      return false;

   for (std::vector<UnicodeString>::const_iterator i =
          items_to_check_for_.begin();
        all_present && i != items_to_check_for_.end(); ++i)
      all_present &= present_items.find(i->UpperCase()) != present_items.end();

   return all_present;
}

void
TTableExistencePredicate::retrieve_present_items(
  std::set<UnicodeString> &present_items)
const
{
   utilities().GetTableNames(present_items);
}

void
TGeneratorExistencePredicate::retrieve_present_items(
  std::set<UnicodeString> &present_items)
const
{
   utilities().GetGeneratorNames(present_items);
}

void
TStoredProcedureExistencePredicate::retrieve_present_items(
  std::set<UnicodeString> &present_items)
const
{
   utilities().GetStoredProcedureNames(present_items);
}

void
TViewExistencePredicate::retrieve_present_items(
  std::set<UnicodeString> &present_items)
const
{
   utilities().GetViewNames(present_items);
}

TFieldExistencePredicate::TFieldExistencePredicate(
  const UnicodeString &table_to_check,
  const std::vector<UnicodeString> &fields_to_check_for,
  const TDBUtilities &utilities)
  : TExistencePredicate(utilities)
{
   fields_to_check_for_[table_to_check] = fields_to_check_for;
}

bool
TFieldExistencePredicate::operator()()
const
{
   bool all_present = true;
   std::map<UnicodeString, std::vector<UnicodeString> >::const_iterator i =
     fields_to_check_for_.begin();

   if (!fields_to_check_for_.size())
      return false;

   for (std::map<UnicodeString,
                 std::vector<UnicodeString> >::const_iterator i =
          fields_to_check_for_.begin(); i != fields_to_check_for_.end(); ++i)
     all_present &= check_fields(i->first, i->second);

   return all_present;
}

bool
TFieldExistencePredicate::check_fields(
  const UnicodeString &table_to_check,
  const std::vector<UnicodeString> &fields_to_check_for)
const
{
   bool all_present = true;
   std::set<UnicodeString> present_fields;

   utilities().GetFieldNames(table_to_check, present_fields);
   if (!present_fields.size())
      return false;

   for (std::vector<UnicodeString>::const_iterator i =
          fields_to_check_for.begin();
        all_present && i != fields_to_check_for.end(); ++i)
      all_present &=
        present_fields.find(i->UpperCase()) != present_fields.end();

   return all_present;
}

bool
ExecutePredicateActionIf(TPredicateAction &execute_this,
						 TPredicate &if_this_is_true,
						 TDBTransaction &transaction)
{
   bool is_true = if_this_is_true() == 0;

   if (is_true)
	  execute_this(transaction);

   return is_true;
}

} /* SqlPredicates */

