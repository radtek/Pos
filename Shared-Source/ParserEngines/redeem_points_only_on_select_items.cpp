#include <MenumateVersionParser.h>
#include "redeem_points_only_on_select_items.hh"
#include <Predicates.h>

namespace MenumateVersionParser {
namespace RedeemPointsOnlyOnSelectItems {

using Database::TDBControl;
using Database::TDBTransaction;
using Database::TDBUtilities;

using SqlPredicates::TFieldExistencePredicate;

static bool are_fields_present(TDBControl &);
static PARSER_ERROR insert_new_fields(TDBControl &);
static PARSER_ERROR insert_new_version(TDBControl &);
static bool is_version_present(TDBControl &);

PARSER_ERROR
apply_update(
    TDBControl * const control)
{
        if (has_already_been_applied(control) == true)
                return PE_VERSION_ALREADY_APPLIED;
        else if (insert_new_fields(*control) != PE_VERSION_UPGRADE_FAILED
                 && insert_new_version(*control) != PE_VERSION_UPGRADE_FAILED)
            return PE_VERSION_UPGRADE_SUCCEED;

        return PE_VERSION_UPGRADE_FAILED;
}

bool
has_already_been_applied(
    TDBControl * const control)
{
        return are_fields_present(*control)
               & is_version_present(*control);
}

bool
are_fields_present(
    TDBControl &control)
{
        std::vector<UnicodeString> fields;
        TDBUtilities utilities(control);

        fields.push_back("can_be_paid_for_using_points");
        return TFieldExistencePredicate("itemsize", fields, utilities)();
}

PARSER_ERROR
insert_new_fields(
    TDBControl &control)
{
        try {
                TDBTransaction tr(control);
                TIBSQL *q = tr.Query(tr.AddQuery());
                q->SQL->Text = "alter table itemsize "
                               "      add can_be_paid_for_using_points "
                               "          smallint "
                               "          default 1 "
                               "          not null;";
                tr.StartTransaction();
                q->ExecQuery();
                tr.Commit();
                return PE_VERSION_UPGRADE_SUCCEED;
        } catch (...) {
                return PE_VERSION_UPGRADE_FAILED;
        }
}

/*
 * The macro's needed as the compiler always says that
 * insertVersionNumberQuery(...) is undefined if I pass it any kind of
 * parameter.
 */
#define VERSION_NUMBER "5.24"

PARSER_ERROR
insert_new_version(
    TDBControl &control)
{
        if (is_version_present(control) == true)
                return PE_VERSION_ALREADY_APPLIED;

        try {
                TDBTransaction tr(control);
                TIBSQL *q = tr.Query(tr.AddQuery());

                tr.StartTransaction();
                q->SQL->Text =
                    insertVersionNumberQuery(VERSION_NUMBER).c_str();
                q->ExecQuery();
                tr.Commit();
                return PE_VERSION_UPGRADE_SUCCEED;
        } catch (...) {
                return PE_VERSION_UPGRADE_FAILED;
        }
}

bool
is_version_present(
    TDBControl &control)
{
        try {
                TDBTransaction tr(control);
                TIBSQL *q = tr.Query(tr.AddQuery());

                tr.StartTransaction();
                q->SQL->Text = versionNumberQuery().c_str(); 
                q->ParamByName("versionNumber")->AsString = VERSION_NUMBER;
                q->ExecQuery();
                return q->RecordCount > 0;
        } catch (...) {
                return false;
        }
}

#undef VERSION_NUMBER

} /* RedeemPointsOnlyOnSelectItems */
} /* MenumateVersionParser */

