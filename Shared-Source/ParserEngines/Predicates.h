#ifndef __h_predicates__
#define __h_predicates__

/* Needed for UnicodeString. */
#include <System.hpp>

#include <map>
#include <set>
#include <vector>

namespace Database {
  class TDBTransaction;
  class TDBUtilities;
} /* Database */

namespace SqlPredicates {

using Database::TDBUtilities;
using Database::TDBTransaction;

class TPredicate {
public:
   virtual bool operator()() const = 0;

protected:
   TPredicate() { };
   virtual ~TPredicate() { };
};

class TPredicateAction {
public:
   virtual ~TPredicateAction() { };

   virtual void operator()(TDBTransaction &transaction) = 0;

protected:
   TPredicateAction() { };
};

class TExistencePredicate : public TPredicate {
protected:
   TExistencePredicate(const TDBUtilities &utilities)
     : utilities_(utilities) { };
   virtual ~TExistencePredicate() { };

   const TDBUtilities &
   utilities() const
   {
      return utilities_;
   };

private:
   const TDBUtilities &utilities_;
};

class TSimpleExistencePredicate : public TExistencePredicate {
public:
   bool operator()() const;

protected:
   TSimpleExistencePredicate(
     const std::vector<UnicodeString> &items_to_check_for,
     const TDBUtilities &utilities)
     : TExistencePredicate(utilities),
       items_to_check_for_(items_to_check_for) { };
   virtual ~TSimpleExistencePredicate() { };

private:
   virtual void retrieve_present_items(
     std::set<UnicodeString> &present_items) const = 0;

   const std::vector<UnicodeString> &items_to_check_for_;
};

class TTableExistencePredicate : public TSimpleExistencePredicate {
public:
   TTableExistencePredicate(
     const std::vector<UnicodeString> &tables_to_check_for,
     const TDBUtilities &utilities)
     : TSimpleExistencePredicate(tables_to_check_for, utilities) { };
   ~TTableExistencePredicate() { };

private:
   void retrieve_present_items(std::set<UnicodeString> &present_items) const;
};

class TViewExistencePredicate : public TSimpleExistencePredicate {
public:
   TViewExistencePredicate(
     const std::vector<UnicodeString> &tables_to_check_for,
     const TDBUtilities &utilities)
     : TSimpleExistencePredicate(tables_to_check_for, utilities) { };
   ~TViewExistencePredicate() { };

private:
   void retrieve_present_items(std::set<UnicodeString> &present_items) const;
};

class TGeneratorExistencePredicate : public TSimpleExistencePredicate {
public:
   TGeneratorExistencePredicate(
     const std::vector<UnicodeString> &generators_to_check_for,
     const TDBUtilities &utilities)
     : TSimpleExistencePredicate(generators_to_check_for, utilities) { };
   ~TGeneratorExistencePredicate() { };

private:
   void retrieve_present_items(std::set<UnicodeString> &present_items) const;
};

class TStoredProcedureExistencePredicate : public TSimpleExistencePredicate {
public:
   TStoredProcedureExistencePredicate(
	 const std::vector<UnicodeString> &procedures_to_check_for,
	 const TDBUtilities &utilities)
	 : TSimpleExistencePredicate(procedures_to_check_for, utilities) { };
   ~TStoredProcedureExistencePredicate() { };

private:
   void retrieve_present_items(std::set<UnicodeString> &present_items) const;
};

class TFieldExistencePredicate : public TExistencePredicate {
public:
   TFieldExistencePredicate(
	 const UnicodeString &table_to_check,
	 const std::vector<UnicodeString> &fields_to_check_for,
	 const TDBUtilities &utilities);

   TFieldExistencePredicate(
	 const std::map<UnicodeString,
					std::vector<UnicodeString> > &fields_to_check_for,
	 TDBUtilities &utilities)
	 : TExistencePredicate(utilities),
	   fields_to_check_for_(fields_to_check_for)
   {
   };

   ~TFieldExistencePredicate() { };

   bool operator()() const;

private:
   bool check_fields(
	 const UnicodeString &table_to_check,
	 const std::vector<UnicodeString> &fields_to_check_for) const;

   std::map<UnicodeString, std::vector<UnicodeString> > fields_to_check_for_;
};

bool ExecutePredicateActionIf(TPredicateAction &execute_this,
							  TPredicate &if_this_is_true,
							  TDBTransaction &transaction);

} /* SqlPredicates */

#endif /* __h_predicates__ */


