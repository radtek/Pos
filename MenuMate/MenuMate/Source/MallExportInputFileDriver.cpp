//---------------------------------------------------------------------------


#pragma hdrstop

#include "MallExportInputFileDriver.h"
#include <sstream>
#include <string>
//---------------------------------------------------------------------------

#pragma package(smart_init)

__fastcall TMallExportInputFileDriver::TMallExportInputFileDriver()
{
}
//---------------------------------------------------------------------------

void TMallExportInputFileDriver::ReadFromDB(UnicodeString DataQuery, std::map<UnicodeString, UnicodeString> &DataRead)
{
    Database::TDBTransaction DBTransaction(TDeviceRealTerminal::Instance().DBControl);
    DBTransaction.StartTransaction();
    TIBSQL* query = DBTransaction.Query(DBTransaction.AddQuery());
    query->Close();

    query->SQL->Text = DataQuery;
    query->ExecQuery();

    while(!query->Eof)
    {
		MallExportKey = query->FieldByName("MALLEXPORT_KEY")->AsInteger;
        FieldName = query->FieldByName("FIELD_NAME")->AsString;
		StringValue = query->FieldByName("STRING_VALUE")->AsString;
        IntegerValue = query->FieldByName("INTEGER_VALUE")->AsInteger;
        CurrencyValue = query->FieldByName("CURRENCY_VALUE")->AsCurrency;
        TimeStampValue = query->FieldByName("TIMESTAMP_VALUE")->AsDateTime;

        DataRead[FieldName] = CheckValue(StringValue, IntegerValue, CurrencyValue, TimeStampValue, query);
        query->Next();
    }
    DBTransaction.Commit();
}
//---------------------------------------------------------------------------

UnicodeString TMallExportInputFileDriver::CheckValue(UnicodeString StringValue, int IntegerValue,
                                                     Currency CurrencyValue, TDateTime TimeStampValue, TIBSQL* query)
{

    UnicodeString MapVal = "";
    UnicodeString USTimeStampValue = TimeStampValue.DateTimeString();

    if (!(query->FieldByName("STRING_VALUE")->IsNull))
    {
        MapVal = StringValue;
    }
    else if (!(query->FieldByName("INTEGER_VALUE")->IsNull))
    {
        MapVal = IntToStr(IntegerValue);
    }
    else if (!(query->FieldByName("CURRENCY_VALUE")->IsNull))
    {
        MapVal = CurrToStrF((RoundToNearest(CurrencyValue, 0.01, TGlobalSettings::Instance().MidPointRoundsDown)), ffFixed, 2);
//        MapVal = CurrToStrF(CurrencyValue, ffFixed, 2);
    }
    else if (!(query->FieldByName("TIMESTAMP_VALUE")->IsNull))
    {
        MapVal = USTimeStampValue;
    }
    else
    {
        MapVal = "0.00";
    }

    return MapVal;
}
//---------------------------------------------------------------------------
