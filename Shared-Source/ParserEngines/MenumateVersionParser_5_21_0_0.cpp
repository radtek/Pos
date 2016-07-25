#include <MenumateVersionParser.h>
#include <MenumateVersionParser_5_21_0_0.hh>
#include <Predicates.h>

using Database::TDBControl;
using Database::TDBTransaction;
using Database::TDBUtilities;

using SqlPredicates::TFieldExistencePredicate;

namespace MenumateVersionParser {
namespace V5_21_0_0 {

bool are_fields_present(TDBControl &);
bool create_new_fields_if_not_present(TDBControl &);
bool insert_version(TDBControl &);

PARSER_ERROR
apply(
    TDBControl * const db_control)
{
        _errorMsg.clear();

	/*
	 * Just because the version exists in VERSION_HISTORY doesn't mean
	 * that the new fields actually exist. The addition code here is
	 * smart enough to create new fields only if they do not exist.
	 */
	if (create_new_fields_if_not_present(*db_control) == false)
		return PE_VERSION_UPGRADE_FAILED;

	/*
	 * We do, however, have to insert the version number if it's
	 * already there. We also have to keep up appearences and tell the
	 * parser when "the update was rerun."
	 */
	if (insert_version(*db_control) == true) {
                _errorMsg.append("Updates Have Been Succesfully Applied.");
		return PE_VERSION_UPGRADE_SUCCEED;
        }

	_errorMsg.append("Updates Already Applied Succesfully.");
	return PE_VERSION_ALREADY_APPLIED;
}

bool
are_fields_present(
    TDBControl &db_control)
{
	TDBUtilities utilities(db_control);
	std::vector<UnicodeString> fields_to_check_for;

	fields_to_check_for.push_back("available_quantity");
	fields_to_check_for.push_back("default_quantity");
	fields_to_check_for.push_back("warning_quantity");
	fields_to_check_for.push_back("disable_when_count_reaches_zero");

	return TFieldExistencePredicate("itemsize",
					fields_to_check_for,
					utilities)();
}

bool
create_new_fields_if_not_present(
    TDBControl &db_control)
{
	TDBTransaction tr(db_control);

	try {
		TIBSQL *qr;

		/*
		 * We're starting the transaction early as we need to be
		 * able to roll it back in the exception handler. It's
		 * unlikely that TFieldExistencePredicate will throw an
		 * exception but you never know ...
		 */
		tr.StartTransaction();
		if (are_fields_present(db_control) == true)
			return true;

		qr = tr.Query(tr.AddQuery());
		qr->SQL->Text =
		    "alter table itemsize "
		    "    add available_quantity numeric(15, 4) "
		    "        default -1.0 not null, "
		    "    add default_quantity numeric(15, 4) "
		    "        default -1.0 not null, " 
		    "    add warning_quantity numeric(15, 4) "
		    "        default 0.0 not null, "
		    "    add disable_when_count_reaches_zero "
		    "        smallint default 1 not null;";
		qr->ExecQuery();
		tr.Commit();

		return true;
	} catch (...) {
		tr.Rollback();

		return false;
	}
}

#define VERSION "5.21"

bool
insert_version(
    TDBControl &db_control)
{
	TDBTransaction tr(db_control);
	TIBSQL *qr = tr.Query(tr.AddQuery());

	qr->SQL->Text = versionNumberQuery().c_str();

        tr.StartTransaction();

	qr->ParamByName("versionNumber")->AsString = VERSION;
	if (qr->ExecQuery(), qr->RecordCount != 0) {
		tr.Commit();

		return false;
	}

	qr->Close();
	qr->SQL->Text = insertVersionNumberQuery(VERSION).c_str();
	qr->ExecQuery();
	tr.Commit();

	return true;
}

} /* V5_21_0_0 */
} /* MenumateVersionParser */

