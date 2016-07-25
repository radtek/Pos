//---------------------------------------------------------------------------

#ifndef MallExportInputFileDriverH
#define MallExportInputFileDriverH

#include "MallExportIInput.h"
#include <Classes.hpp>
#include <System.hpp>
#include <map>
#include "MM_DBCore.h"
#include "DeviceRealTerminal.h"
//---------------------------------------------------------------------------
class TMallExportInputFileDriver : public TMallExportIInput
{
public:
    void ReadFromDB(UnicodeString DataQuery, std::map<UnicodeString, UnicodeString> &DataRead);
    UnicodeString CheckValue(UnicodeString StringValue, int IntegerValue, Currency CurrencyValue,
                    TDateTime TimeStampValue, TIBSQL* query);
//    UnicodeString CheckValue(UnicodeString StringValue, UnicodeString IntegerValue, UnicodeString CurrencyValue,
//                    TDateTime TimeStampValue, TIBSQL* query);
//        UnicodeString MallExportKey;
//        UnicodeString IntegerValue;
        int MallExportKey;
        int IntegerValue;
        UnicodeString FieldName;
        UnicodeString StringValue;
//        UnicodeString CurrencyValue;
        Currency CurrencyValue;
        TDateTime TimeStampValue;

    __fastcall TMallExportInputFileDriver();
};
#endif
