//---------------------------------------------------------------------------

#ifndef ReportExportInterfaceH
#define ReportExportInterfaceH
#include <System.hpp>
#include <vector>
#include <IBDatabase.hpp>
#include "MM_DBCore.h"
//---------------------------------------------------------------------------
class TReportExportInterface
{
	public:
	virtual void PostDataToFile() = 0;
    virtual std::vector<UnicodeString> CreateHeaderFormat(Database::TDBTransaction &dbTransaction, TDateTime SDate, TDateTime EDate,
                                                    bool isAllTerminalSelected) = 0;
    virtual std::vector<UnicodeString> PrepareDataForExport(Database::TDBTransaction &dbTransaction, TDateTime SDate, TDateTime EDate,
                                                    bool isAllTerminalSelected) = 0;
};
#endif
