#include "DB_5_15.h"
#include "MM_DBCore.h"
#include "Predicates.h"

using Database::TDBControl;
using Database::TDBUtilities;
using Database::TDBTransaction;

void
DB_5_15::Apply(TDBControl &control)
{
    TDBTransaction t(control);
    TIBSQL *q = t.Query(t.AddQuery());

    q->SQL->Text = L"alter table chitnumber drop default_menu;";
    t.StartTransaction();
        q->ExecQuery();
        q->Close();
        q->SQL->Text =
          "alter table chitnumber add default_menu varchar(15) default null;";
        q->ExecQuery();
    t.Commit();
}

