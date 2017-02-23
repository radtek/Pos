//---------------------------------------------------------------------------

#ifndef HavanaReportH
#define HavanaReportH
#include "DeviceRealTerminal.h"
//---------------------------------------------------------------------------

class THavanaReport
{
    private :

    public:
    THavanaReport();
    std::vector<UnicodeString> CreateHeaderFormat(Database::TDBTransaction &dbTransaction, TDateTime SDate, TDateTime EDate);
    std::vector<UnicodeString> LoadDataFromDB(Database::TDBTransaction &dbTransaction, TDateTime SDate, TDateTime EDate);

};
#endif
