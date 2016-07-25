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
	~MallExportConsolidatedReceipt();

	virtual void GetOutput(TPrintout* printout);

protected:

    void AddTitleWithColumn(TPrintout* printOut, AnsiString title1, AnsiString title2);
    AnsiString GetSalesDetails();

private:
    DataFormatUtilities* dataFormatUtilities;
    DataCalculationUtilities* dataCalculationUtilities;
    ReportFinancialCalculations* reportFinancialCalculations;
};
#endif
