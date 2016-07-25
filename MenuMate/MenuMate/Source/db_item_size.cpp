#include <cassert>

#pragma option push
#pragma option -w-dup \
               -w-hid \
               -w-mls \
               -w-pch
#        include "DeviceRealControl.h"
#        include "MM_DBCore.h"
#pragma option pop

#include "db_item_size.h"

using Database::TDBTransaction;

int
db_item_size_t::default_patron_count(
    const db_item_key_types::item_key_t item_key,
    const db_item_key_types::item_size_name_t &size_name)
{
        TDBTransaction tr(
            TDeviceRealControl::ActiveInstance().DBControl);
        TIBSQL *q = tr.Query(tr.AddQuery());
        int count = 0;

        q->SQL->Text = "select default_patron_count "
                       "       from itemsize "
                       "       where item_key = :item_key "
                       "             and size_name = :size_name;";
        tr.StartTransaction();
                q->ParamByName("item_key")->AsInteger = item_key;
                q->ParamByName("size_name")->AsString = size_name;
                if (q->ExecQuery(), q->RecordCount != 0)
                        count =
                            q->FieldByName("default_patron_count")->AsInteger;
        tr.Commit();

        return count;
}

