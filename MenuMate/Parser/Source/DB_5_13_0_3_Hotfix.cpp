#include "DB_5_13_0_3_Hotfix.h"
#include "MM_DBCore.h"
#include "Predicates.h"

using Database::TDBControl;
using Database::TDBUtilities;
using Database::TDBTransaction;

using SqlPredicates::TFieldExistencePredicate;

void
DB_5_13_0_3_Hotfix::Apply(TDBControl &control)
{
    TDBUtilities utilities(control);
    std::vector<UnicodeString> fields_to_check_for;

    fields_to_check_for.push_back("applytomembers");
    if (!TFieldExistencePredicate("orderdiscounts",
                                  fields_to_check_for,
                                  utilities)()) {
        TDBTransaction t(control);
        TIBSQL *q = t.Query(t.AddQuery());
        q->SQL->Text =
          "alter table orderdiscounts "
          "      add applytomembers T_TRUEFALSE default 'F' not null;";
        t.StartTransaction();
        try {
            q->ExecQuery();
            t.Commit();
        } catch (...) {
            t.Rollback();
            throw;
        }
    }
}

