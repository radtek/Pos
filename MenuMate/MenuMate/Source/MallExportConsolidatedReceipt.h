//---------------------------------------------------------------------------

#ifndef MallExportConsolidatedReceiptH
#define MallExportConsolidatedReceiptH

#include "BaseReportSection.h"
#include "MM_DBCore.h"
#include "GlobalSettings.h"
#include "Printout.h"
#include "ReportUtilities.h"
#include "ReportFinancialCalculations.h"
//---------------------------------------------------------------------------

class MallExportConsolidatedReceipt : public BaseReportSection
{
public:
	MallExportConsolidatedReceipt(Database::TDBTransaction* dbTransaction, TGlobalSettings* globalSettings);
    MallExportConsolidatedReceipt(Database::TDBTransaction* dbTransaction, TGlobalSettings* globalSettings, TDateTime* startTime, TDateTime* endTime);
	~MallExportConsolidatedReceipt();

	virtual void GetOutput(TPrintout* printout);
    virtual void GetOutput(TPrintout* printout, TDateTime* startTime, TDateTime* endTime);

protected:

    void AddTitleWithColumn(TPrintout* printOut, AnsiString title1, AnsiString title2);
    int GetSalesDetails();
    UnicodeString FixDecimalPlaces(Currency AmountTotal);

private:
    DataFormatUtilities* dataFormatUtilities;
    DataCalculationUtilities* dataCalculationUtilities;
    ReportFinancialCalculations* reportFinancialCalculations;
};
#endif
