//---------------------------------------------------------------------------

#ifndef ZWaiterAppOrderDetailsReportSectionH
#define ZWaiterAppOrderDetailsReportSectionH
#include "BaseReportSection.h"
#include "MM_DBCore.h"
#include "Printout.h"
#include "ReportUtilities.h"


class ZWaiterAppOrderDetailsReportSection : public BaseReportSection
{
public:
	ZWaiterAppOrderDetailsReportSection(Database::TDBTransaction* dbTransaction, TGlobalSettings* globalSettings);
	~ZWaiterAppOrderDetailsReportSection();

	virtual void GetOutput(TPrintout* printout);

private:
    DataFormatUtilities* dataFormatUtilities;
    DataCalculationUtilities* dataCalculationUtilities;
    void PrintHeader(UnicodeString textToPrint, TPrintout &printOut);
    void PrintItemRow(TWaiterAppOrderInfo waiterAppOrderInfo, UnicodeString &itemName, TPrintout &printOut);
    void PrintColumnHeader(TPrintout &printOut);
    void AddDoubleLine(TPrintout &printOut);
    void PrintOrderInformation(std::list<TWaiterAppOrderInfo> waiterAppOrderInfoList, UnicodeString itemName, TPrintout* printout);
    void PrintSectionHeader(UnicodeString sectionName, TPrintout* printout);
    void PrintReportHeader(UnicodeString title, UnicodeString subTitle, TPrintout* printOut);

};
#endif
