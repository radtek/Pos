#pragma hdrstop

#include "DB_5_12_0_0_ChitList.h"
#include "MM_DBCore.h"
#include "Predicates.h"

#pragma package(smart_init)

using Database::TDBTransaction;
using Database::TDBControl;
using Database::TDBUtilities;

using namespace SqlPredicates;

static void
add_archival_fields(TDBControl &control, TDBTransaction &tr)
{
    std::vector<UnicodeString> fields;
    std::vector<UnicodeString> sql_statements;
    std::vector<UnicodeString> tables;
    TDBUtilities utilities(control);
    TIBSQL *q = tr.Query(tr.AddQuery());

    fields.push_back("chit_name");
    fields.push_back("chit_option");
    tables.push_back("archive");
    tables.push_back("dayarchive");

    for (std::vector<UnicodeString>::const_iterator i = tables.begin();
           i != tables.end(); ++i)
        if (!TFieldExistencePredicate(*i, fields, utilities)()) {
            q->SQL->Text = "alter table "
                           + *i
                           + " add chit_name varchar(32) default null, "
                           + " add chit_option varchar(32) default null;";
            q->ExecQuery();
            q->Close();
        }
}

static void
add_chitnumber_fields(TDBControl &control, TDBTransaction &tr)
{
    std::vector<UnicodeString> fields;
    TDBUtilities utilities(control);
    TIBSQL *q = tr.Query(tr.AddQuery());

    fields.push_back("default_menu");
    if (TFieldExistencePredicate("chitnumber", fields, utilities)())
        return;

    q->SQL->Text = "alter table chitnumber add default_menu int default null;";
    q->ExecQuery();
    q->Close();
}

static void
add_chitoptions_table(TDBControl &control, TDBTransaction &tr)
{
    std::vector<UnicodeString> tables;
    TDBUtilities utilities(control);
    TIBSQL *q = tr.Query(tr.AddQuery());

    tables.push_back("chitoptions");
    if (TTableExistencePredicate(tables, utilities)())
        return;

    q->SQL->Text = "create table chitoptions ("
                   "  chitoptions_key int not null, "
                   "  chitnumber_key int not null, "
                   "  name varchar(32) not null, "
                   "  constraint chitoptions_pk primary key(chitoptions_key), "
                   "  constraint chitnumber_fk foreign key(chitnumber_key) "
                   "             references chitnumber(chitnumber_key) "
                   "             on delete cascade "
                   "             on update cascade, "
                   "  constraint chitoptions_u unique(chitnumber_key, name) "
                   ");";
    q->ExecQuery();
    q->Close();
}

static void
add_chitoptions_generator(TDBControl &control, TDBTransaction &tr)
{
    std::vector<UnicodeString> generators;
    TDBUtilities utilities(control);
    TIBSQL *q = tr.Query(tr.AddQuery());

    generators.push_back("gen_chitoptions");
    if (TGeneratorExistencePredicate(generators, utilities)())
        return;

    q->SQL->Text = "create generator gen_chitoptions;";
    q->ExecQuery();
}

void
DB_5_12_0_0_ChitList::Up(TDBControl &control)
{
    TDBTransaction tr(control);

    tr.StartTransaction();
        add_archival_fields(control, tr);
        add_chitnumber_fields(control, tr);
        add_chitoptions_table(control, tr);
        add_chitoptions_generator(control, tr);
    tr.Commit();
}

