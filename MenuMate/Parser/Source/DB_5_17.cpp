#include "DB_5_17.h"
#include "MM_DBCore.h"
#include "Predicates.h"

using Database::TDBControl;
using Database::TDBUtilities;
using Database::TDBTransaction;

void
DB_5_17::Apply(TDBControl &control)
{
    TDBTransaction t(control);
    TIBSQL *q = t.Query(t.AddQuery());

	t.StartTransaction();
	q->SQL->Text = "alter table chitnumber add disable_hold_and_send char(1) default 'F';";
	q->ExecQuery();
	q->Close();
    t.Commit();
}
