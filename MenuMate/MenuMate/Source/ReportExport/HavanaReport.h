//---------------------------------------------------------------------------

#ifndef HavanaReportH
#define HavanaReportH
#include "DeviceRealTerminal.h"
//---------------------------------------------------------------------------

class THavanaReport
{
    private :
    std::map<UnicodeString,UnicodeString> paymentTypes;
    std::map<UnicodeString,UnicodeString> menuNames;
    std::map<UnicodeString,UnicodeString> LoadAllPaymentTypes(Database::TDBTransaction &dbTransaction, TDateTime SDate, TDateTime EDate,
                                                    bool isAllTerminalSelected);
    std::map<UnicodeString,UnicodeString> LoadAllMenus(Database::TDBTransaction &dbTransaction, TDateTime SDate, TDateTime EDate,
                                                    bool isAllTerminalSelected);

    public:
    THavanaReport();
    std::vector<UnicodeString> CreateHeaderFormat(Database::TDBTransaction &dbTransaction, TDateTime SDate, TDateTime EDate,
                                                    bool isAllTerminalSelected);
    std::vector<UnicodeString> LoadDataFromDB(Database::TDBTransaction &dbTransaction, TDateTime SDate, TDateTime EDate,
                                                    bool isAllTerminalSelected);

};
#endif
