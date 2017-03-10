//---------------------------------------------------------------------------

#ifndef HavanaReportH
#define HavanaReportH
#include "DeviceRealTerminal.h"
#include "MallExportOutputDBDriver.h"
#include "ReportExportInterface.h"
#include "MallExportRegenerateReport.h"
//---------------------------------------------------------------------------

class THavanaReport : public TReportExportInterface
{
    private :
    UnicodeString innerQuery;
    std::map<UnicodeString,UnicodeString> paymentTypes;
    std::map<UnicodeString,UnicodeString> paymentTypeTotal;
    std::map<UnicodeString,UnicodeString> menuNames;
    std::map<UnicodeString,UnicodeString> menuTotals;
    std::map<UnicodeString,UnicodeString> LoadAllPaymentTypes(Database::TDBTransaction &dbTransaction, TDateTime SDate, TDateTime EDate,
                                                    bool isAllTerminalSelected);
    std::map<UnicodeString,UnicodeString> LoadAllMenus(Database::TDBTransaction &dbTransaction, TDateTime SDate, TDateTime EDate,
                                                    bool isAllTerminalSelected);

    UnicodeString RemoveCommas(UnicodeString value);

    void CreateDirectory(UnicodeString &path, TDateTime SDate, TDateTime EDate);

    void PrepareDataForCSVFile(TDateTime SDate, TDateTime EDate, UnicodeString reportExportPath, bool isAllTerminalSelected);

    public:
    THavanaReport();
    void PostDataToFile();
    std::vector<UnicodeString> CreateHeaderFormat(Database::TDBTransaction &dbTransaction, TDateTime SDate, TDateTime EDate,
                                                    bool isAllTerminalSelected);
    std::vector<UnicodeString> PrepareDataForExport(Database::TDBTransaction &dbTransaction, TDateTime SDate, TDateTime EDate,
                                                    bool isAllTerminalSelected);
};
#endif
